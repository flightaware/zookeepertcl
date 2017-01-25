#
# zookeeper support functions
#
#

namespace eval ::zookeeper  {

	#
	# mkpath - make all the znodes
	# in the path that don't exist
	#
	proc mkpath {zk path} {
		if {[$zk exists $path]} {
			return
		}
		mkpath $zk [file dirname $path]
		$zk create $path
	}

	proc rmrf {zk path} {
		foreach child [$zk children $path] {
			rmrf $zk $path/$child
		}
		$zk delete $path -1
	}

	proc read_file {file} {
		set fp [open $file]
		set data [read $fp]
		close $fp
		return $data
	}

	#
	# copy_file - copy a file
	#
	proc copy_file {zk file zpath} {
		set data [read_file $file]
		copy_data $zk $data $zpath
	}

	#
	# copy_data - copy data
	#
	proc copy_data {zk data zpath} {
		if {![$zk exists $zpath]} {
			# the node doesn't exist, create it
			$zk create $zpath -value $data
			return
		}

		# node exists, but it may not have any data in it.
		# if it has data, compare that and if it's the
		# same, do nothing.
		#
		# otherwise, set the data

		if {[$zk get $zpath -data zdata -version zversion]} {
			# if it matches the file, leave it alone
			if {$data == $zdata} {
				return
			}
		} else {
			set zversion 0
		}

		# file is different set the znode to contain it
		$zk set $zpath $data $zversion
	}

	#
	# zsync - sync a filesystem tree to a znode tree
	#
	# zpath is prepended to the destination path
	#
	proc zsync {zk path zpath {pattern *}} {
		mkpath $zk $zpath
		set regexp "^${path}(.*)"
		foreach file [lsort [glob -nocomplain -dir $path -types f $pattern]] {
			regexp $regexp $file dummy tail
			copy_file $zk $file $zpath$tail
		}
		foreach dir [lsort [glob -nocomplain -dir $path -types d *]] {
			regexp $regexp $dir dummy tailDir
			zsync $zk $dir $zpath$tailDir $pattern
		}
	}

	#
	# zcopy - copy a znode tree to a filesystem
	#
	# zpath is prepended to the destination path
	#
	proc zcopy {zk zpath path {pattern *}} {
		file mkdir $path
		set regexp "^${path}(.*)"
		set children [$zk children $zpath]
		foreach znode $children {
			regexp $regexp $file dummy tail
			copy_file $zk $file $zpath$tail
			regexp $regexp $dir dummy tailDir
			zcopy $zk $dir $zpath$tailDir $pattern
		}
	}
} ;# namespace ::zookeeper

# vim: set ts=4 sw=4 sts=4 noet :


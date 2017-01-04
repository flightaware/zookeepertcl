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
		set data [read $file]
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
		# get the node
		if {[$zk get $zpath -data zdata -version zversion]} {
			# the node exists
			# if it matches the file, leave it alone
			if {$data == $zdata} {
				return
			}
			# file is different set the znode to contain it
			$zk set $zpath $data $zversion
		} else {
			# the node doesn't exist, create it
			$zk create $path -value $data
		}
	}
} ;# namespace ::zookeeper

# vim: set ts=4 sw=4 sts=4 noet :


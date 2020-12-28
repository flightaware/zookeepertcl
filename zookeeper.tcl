#
# zookeeper support functions
#
#

package require base64
package require sha1
package require struct::set

namespace eval ::zookeeper  {
	variable zkwd "/"

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

	proc write_file {file data} {
		set fp [open $file w]
		puts -nonewline $fp $data
		close $fp
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
			if {[info exists zdata] && $data == $zdata} {
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
	#	removes any items in zk not found in the local
	#	path
	#
	# zpath is prepended to the destination path
	#
	proc zsync {zk path zpath {pattern *}} {
		mkpath $zk $zpath
		#set regexp "^${path}(.*)"
		set locals [list]
		foreach file [lsort [glob -nocomplain -dir $path -types f $pattern]] {
			#regexp $regexp $file dummy tail
			set tail [file tail $file]
			copy_file $zk $file [file join $zpath $tail]
			lappend locals $tail
		}
		foreach dir [lsort [glob -nocomplain -dir $path -types d *]] {
			#regexp $regexp $dir dummy tailDir
			set tailDir [file tail $dir]
			zsync $zk $dir [file join $zpath $tailDir] $pattern
			lappend locals $tailDir
		}
		set nodesToRemove [::struct::set difference [$zk children $zpath] $locals]
		#puts stderr "path: $path zpath: $zpath nodesToRemove: $nodesToRemove dirs + files: $locals children: [$zk children $zpath]"
		foreach node $nodesToRemove {
			puts stderr "Removing [file join $zpath $node]"
			rmrf $zk [file join $zpath $node]
		}
	}

	#
	# sync_znode_to_file - sync the data at zpath to
	#   path/zpath/Zdata and Zversion, if there is
	#   data at that znode.
	#
	proc sync_znode_to_file {zk zpath path} {
		set outpath $path/$zpath
		set zdataFile $outpath/Zdata
		set zversionFile $outpath/Zversion

		set exists 1
		try {
			file stat $zdataFile stat
		} trap {POSIX ENOENT} {} {
			set exists 0
		}

		if {![$zk get $zpath -data zdata -version zversion] || ![info exists zdata]} {
			# there is no data at the znode.  if there is a file,
			# delete it.
			if {$exists} {
				file delete $zdataFile $zversionFile
			}
			return
		}

		# there was data at the znode.
		# if it matches what we have, no need to write.
		if {$exists && $stat(size) == [string length $zdata] && [read_file $zdataFile] eq $zdata && [read_file $zversionFile] eq $zversion} {
			#puts stderr "skip writing $zdataFile, existing one matches"
			return
		}

		file mkdir $outpath
		if {!$exists} {
			# files didn't exist, we can write them without renaming
			write_file $zdataFile $zdata
			write_file $zversionFile $zversion
		} else {
			# we need to write and the files exist, do it more cleanly
			# so a reader won't see an empty file if they look at just
			# the right time.
			# NB still a race condition here where the zdata is updated
			# but very briefly the zversion isn't.
			write_file $zdataFile.new $zdata
			write_file $zversionFile.new $zversion
			file rename -force -- $zdataFile.new $zdataFile
			file rename -force -- $zversionFile.new $zversionFile
		}
	}

	#
	# sync_ztree_to_filetree - copy a znode tree to a filesystem
	#
	# zpath is prepended to the destination path
	#
	proc sync_ztree_to_directory {zk zpath path} {
		#puts stderr "sync_ztree_to_directory $zk $zpath $path"
		sync_znode_to_file $zk $zpath $path

		# recursively invoke ourself for all children of the current znode
		set children [$zk children $zpath]
		foreach znode $children {
			sync_ztree_to_directory $zk [file join $zpath $znode] $path
		}
	}

	proc zls {{where ""}} {
		variable zkwd

		set children [zk children [file join $zkwd $where]]
		return [lsort $children]
	}

	proc zpwd {} {
		variable zkwd

		return $zkwd
	}

	proc zcd {{where ""}} {
		variable zkwd

		if {$where eq ""} {
			set zkwd /
		} else {
			set children
		}
		return
	}

	#
	# zdigest_id - given a username and password for use with the digest
	#  ACL scheme, return the id portion of the ACL, which is
	#  username:[zdigest password]
	#
	proc zdigest_id {un pw} {
		set digest [base64::encode [sha1::sha1 -bin "${un}:${pw}"]]
		return [format {%s:%s} $un $digest]
	}

} ;# namespace ::zookeeper

# vim: set ts=4 sw=4 sts=4 noet :


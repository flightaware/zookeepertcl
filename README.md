[![Build Status](https://travis-ci.org/flightaware/zookeepertcl.svg?branch=master)](https://travis-ci.org/flightaware/zookeepertcl)

zookeepertcl, a Tcl interface to the Apache zookeeper distributed coordination system
===

zookeepertcl provides a Tcl interface to the Apache Zookeeper C language API.

Functionality
---

- Provides a natural Tcl interface
- Fast
- Synchronous interface for simplicity and convenience
- Asynchronous interface for performance
- Thread safe
- Free!

License
---

Open source under the permissive Berkeley copyright; see file [LICENSE](LICENSE).

Requirements
---
Requires the Apache Zookeeper C/C++ library [libzookeeper](https://zookeeper.apache.org/doc/current/zookeeperProgrammers.html) be installed.

Building
---

```sh
autoconf
configure
make
sudo make install
```

For FreeBSD, something like

```sh
./configure --with-tcl=/usr/local/lib/tcl8.6  --mandir=/usr/local/man --enable-symbols
```

Accessing from Tcl
---

```tcl
package require zookeeper
```

Versions
---


Overview
---

```tcl
package require zookeeper

zookeeper::zookeeper debug_level debug
```

debug level can be debug, info, warn, error or none.

`zookeeper::zookeeper version` returns the version of the C client, like **3.4.6**.  (The version of zookeeper Tcl can always be determined using `package require zookeeper` or one of various other Tcl package methods.)

zookeeper::zookeeper init name host timeout ?-async callback?

```tcl
set zk [zookeeper init #auto localhost:2181 50000]
```

In the "#auto" style, when `zookeeper init` is invoked, a new zookeeper object is created with a unique, generated name... something like *zookeeper0* and you use it by saving a reference to it in a variable or whatever.  In this style we invoke methods on the zookeeper object using **$zk**.

```tcl
zookeeper::zookeeper init zk localhost:2181 50000
```

In the explicitly named style, in place of **#auto** you provide the name of the command, in this case, **zk**.

If **-async** is specified, what follows next is a command that will be executed for general zookeeper callbacks.  While watches, asynchronous data requests, etc, have their own callbacks, this callback will tell you the state is connected and stuff like that.

Creating a znode is simple...

```tcl
zk create /test -value woof
```

Option switches provide additional capabilities.

```tcl
set znode [zk create /k -value woof -ephemeral]
zk get $znode
```

This creates, sets, and fetches the contents of an *ephemeral node* that only lasts for the life of the process, on zookeeper.  You need to use the znode ID returned in place of the one you specified the name of the znode is altered by zookeeper based on what you asked for.  Like /k becomes /k00000000.  (I think this change only occurs if -ephemeral or -sequence is used.)

```tcl
zk create path ?-value value? ?-ephemeral? ?-sequence? ?-async callback?
```

Create the path.  Value, if provided, is set as the value at the path else the znode's value is left as null.  **-ephemeral** makes the path exist only for the life of the connection from this process in accordance with normal zookeeper behavior.  If **-sequence** is provided, a unique monotonically increasing sequence number is appended to the pathname.  This can be very handy for the kinds of things zookeeper is typically used for.  Please investigate general zookeeper documentation for more details.

If **-async** is provided, *callback* will be invoked with a list of key-value pairs containing the status of the operation once it is complete.

```tcl
zk get $path ?-watch code? ?-stat array? ?-async callback? ?-data dataVar? ?-version versionVar?
```
Get the data at znode *$path*.  A watch is set if the znode exists and **-watch** is specified; code is invoked when the znode is changed, with an argument of a list of key-value pairs about the watched object.  If **-stat** is specified, *array* is the name of an array that is filled with stat data such as *version* and some other stuff.

If **-data** is specified, *dataVar* is the name of an array that the data is stored into, and get returns 1 if the znode exists and 0 if it doesn't.  (Without the -data option, the value is returned or an error is thrown if the znode doesn't exist.)

When **-version** is specified, *versionVar* is stored with the version number of the znode if the znode exists.

If **-async** is specified, code is executed as a callback when the result has come in from zookeeper.  The callback will be invoked with an argument consisting of a Tcl list of key-value pairs.  The name of the zookeeper object will be in *zk*, the status (like *ZOK*), in status, and if there is data attached to the znode, the data as *data* and version as *version*.

It is an error to try to specify -data, -version or -stat along with -async.

```tcl
zk exists path ?-watch code? ?-stat array? ?-async callback? ?-version versionVar?
```

Return 1 if the path exists and 0 if it doesn't.  **-watch**, **-stat** and **-version** are the same as for **get** above.

If **-async** is specified, the request is made asynchronously and *callback* is invoked with a Tcl list of key-value pairs as an argument when the answer arrives.

Neither -watch, -stat or -version can be specified when -async is used.

```tcl
zk children $path ?-async callback? ?-watch code?
```

Return a Tcl list of the names of the child znodes of the given path.  If **-async** is specified, *callback* is invoked once the data arrives, with a list of key-value pairs such as `zk ::zk status ZOK data bark version 0`.  In this case, the zookeeper object is **::zk**, the status is **ZOK**, the data is **bark** and the version is **0**.

If the **-watch** option is specified, *code* is executed if a child node is added or removed.

Remember, watches only fire one time, so they must be set up again if you want them to fire again on another change.

```tcl
zk set $path $data $version ?-async callback?
```

Set the znode at the given path to contain the specified data. Version must match or be **-1** which bypasses the version check.  (It is a best practice to use the versioning.)

Zookeeper supports null data for any znode, and zookeepertcl provides ways to distinguish between a znode with no data and a znode with an empty string as data.  We properly make providing data optional in *create*, so if create is invoked with no **-data** option we leave the data in the znode properly null.

If the **get** method is invoked synchronously with the **-data** option, the specified variable will only be set if there is data associated with the znode.  (Otherwise the variable will be force-unset.)

Also if **get** is used asynchronously then the key-value pairs will not contain a *data* pair if there is no data ssociated with the znode.

```tcl
zk delete $path $version ?-async callback?
```

Delete the path on zookeeper server.  Version must be the right version number in accordance with normal zookeeper rules.  Following the same, if **-1** is used as the version number the version check will not be performed by zookeeper.

If **-async** is specified, the request is made asynchronously and *callback* is invoked with a Tcl list of key-value pairs as an argument when the answer arrives.

```tcl
zk state
```

This returns the state of the zookeeper session.  It can be **closed**, **connecting**, **associating**, **connected**, **expired**, OR **auth_failed**.

```tcl
zk recv_timeout
```

This returns the timeout for the session, in milliseconds.  It's only valid if the state is connected.  zookeeper C API docs say the value may change after a server reconnect.

```tcl
zk is_unrecoverable
```

Return true if the zookeeper C library says the connection state can't be recovered.

If this returns true then the application must destroy the zookeeper object and reconnect.

```tcl
zk destroy
```

Destroys the Zookeeper object, disconnecting from Zookeeper and removing all watch or async callbacks in the process.

Watch Callbacks
---

When a znode that is being watched has changed, the code that you specified is executed with an argument consisting of a list of key-value pairs suitable for putting into an array using `array get`.

The list elements are **path** followed by the znode path, **zk** followed by the name of the zookeepertcl zookeeper command that was used to create the watch, **type** followed by the zookeeper type and **state** followed by the zookeeper state.

State will be one of **closed**, **connecting**, **associating**, **connected**, **expired**, **auth_failed** and **unknown**.

Type will be one of **created**, **deleted**, **changed**, **child**, **session**, **not_watching** or **unknown**.

For example, for a zookeeper object named *zk* that has a watch up on **/test** and that znode is changed, the argument to the callback will be something like

```
zk ::zk path /test type changed state connected
```

Stat Structure
---
When requesting status using **-stat**, the specified array will be filled with the following elements:
* aversion - the number of changes to the ACL of this znode.
* ctime - the time in milliseconds from epoch when this znode was created.
* cversion - the number of changes to the children of this znode.
* czxid - the zookeeper transaction ID (zxid) of the change that caused this znode to be created.
* dataLength - the length of the data field of this znode.
* ephemeralOwner - the session id of the owner of this znode if the znode is an ephemeral znode. If it is not an ephemeral znode, it will be zero.
* mtime - the time in milliseconds from epoch when this znode was last modified.
* mzxid - the zxid of the change that last modified this znode.
* numChildren - the number of children of this znode.
* pzxid - the zxid of the change that last modified children of this znode.
* version - the number of changes to the data of this znode.

zookeeper library functions
---

Also included when you do a `package require zookeeper` are some handy Tcl help functions, created in the zookeeper namespace.

```tcl
::zookeeper::rmrf $zk $path
```

Recursively delete a path and all of its children.  Rather dangerous.

```tcl
zookeeper::mkpath $zk $path
```

Make all the znodes in the specified path that don't already exist.

```tcl
zookeeper::copy_file $zk $file $zpath
```

Copy the specified file to the specified path on zookeeper.

```tcl
zookeeper::copy_data $zk $data $zpath
```

Copy the specified data to the specified path on zookeeper.

```tcl
zookeeper::zsync $zk $path $zpath ?pattern?
```

Sync a filesystem tree to a znode tree.  zpath is prepended to the destination path.  Compares existing files and znode data and if they are present and identical, does not update the znode.  This makes znode versions increment only when changes are present in corresponding files when zsync is run.

```tcl
zookeeper::sync_ztree_to_directory $zk $zpath $path
```

Recursively copy a zookeeper tree to a directory in a filesystem.

All nodes will be created as directories.

If a znode contains data the data will be written in the corresponding directory as the file _zdata_ and the version as the file _zversion_.

The function makes the effort to skip rewriting the data and version files if the existing data and the version files are the same.  (This considerably speeds up the function and reduces filesystem churn.)

Also in the event of changes if there was already a zdata and zversion to renames new files in rather than overwriting them so a reader reading the file at an inopportune moment won't get an empty file.


Errata
---

* ACLs are not currently supported.
* Atomically committed multiple zookeeper operations are not current supported.
* Memory leaks are a distinct possibility.

Links
---

* [Tcl_CreateEventSource, Tcl_SetMaxBlockTime, Tcl_ThreadQueueEvent](https://www.tcl.tk/man/tcl/TclLib/Notifier.htm)
* [Tcl_OpenFileChannel, Tcl_DetachChannel](https://www.tcl.tk/man/tcl/TclLib/OpenFileChnl.htm)
* [Zookeeper wiki](https://cwiki.apache.org/confluence/display/ZOOKEEPER/Index)
* [Github mirror of zookeeper](https://github.com/apache/zookeeper)
* [zookeeper programmer's guide](https://zookeeper.apache.org/doc/trunk/zookeeperProgrammers.html)

FlightAware
---
FlightAware has released over a dozen applications  (under the free and liberal BSD license) into the open source community. FlightAware's repositories are available on GitHub for public use, discussion, bug reports, and contribution. Read more at https://flightaware.com/about/code/

zookeepertcl, a Tcl interface to the Apache zookeeper distributed coordination system
===

zookeepertcl provides a Tcl interface to the Apache Zookeeper C language API.

Functionality
---

- Provides a natural Tcl interface
- Fast
- Asynchronous
- Thread safe
- Free!

License
---

Open source under the permissive Berkeley copyright, see file LICENSE

Requirements
---
Requires the Apache Zookeeper C/C++ library *libzookeeper* be installed.

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
debug level can be debug, info, warn or error.

`zookeeper::zookeeper version`` returns the version of the C client, like **3.4.6**.  (The version of zookeeper Tcl can always be determined using `package require zookeeper` or one of various other Tcl package methods.)

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

Creating a node is simple...

```tcl
zk create /test -value woof
```

Option switches provide additional capabilities.

```tcl
set node [zk create /k -value woof -ephemeral]
zk get $node 0 z
```

This creates, sets, and fetches the contents of an *ephemeral node* that only lasts for the life of the process, on zookeeper.  You need to use the node ID returned in place of the one you specified the name of the node is altered by zookeeper based on what you asked for.  Like /k becomes /k00000000.  (I think this change only occurs if -ephemeral or -sequence is used.)

```tcl
zk create path ?-value value? ?-ephemeral? ?-sequence?
```

Create the path.  Value, if provided, is set as the value at the path else the node's value is left as null.  **-ephemeral** makes the path exist only for the life of the connection from this process in accordance with normal zookeeper behavior.  If **-sequence** is provided, a unique monotonically increasing sequence number is appended to the pathname.  This can be very handy for the kinds of things zookeeper is typically used for.  Please investigate general zookeeper documentation for more details.

```tcl
zk get $path ?-watch code? ?-stat array?
```

Get the data at node *$path*.  A watch is set if **-watch** is specified; code is invoked when the node is changed, with an argument of a list of key-value pairs about the watched object.  If **-stat** is specified, *array* is the name of an array that is filled with stat data such as *version* and some other stuff.

```tcl
zk exists path ?-watch code? ?-stat array?
```

Return 1 if the path exists and 0 if it doesn't.  **-watch** and **-stat** are the same as for **get** above.

```tcl
zk children $path
```

Return a Tcl list of the names of the child znodes of the given path.

(The C API supports adding a watch with this call but we currently do not.)

```tcl
zk set $path $data $version
```

Set the znode at the given path to contain the specified data. Version must match or be **-1** which bypasses the version check.  It is a best practice to use the versioning.

Zookeeper supports null data for any znode, and we have only half-implmeneted that for out-of-band detection.  We properly make providing data optional in *set* and *create*, and we set the data in the znode to null properly.  But to close the loop we have to make a way for *get* to provide an out-of-band result, like having it return 1 or 0 and store the results into a variable passed through an argument or something like that.

```tcl
zk delete $path $version
```

Delete the path on zookeeper server.  Version must be the right version number in accordance with normal zookeeper rules.  Following the same, if **-1** is used as the version number the version check will not be performed by zookeeper.

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

If this returns true then the application must close the zhandle object and try to reconnect.

Watch Callbacks
---

When a znode that is being watched has changed, the code that you specified is executed with an argument consisting of a list of key-value pairs suitable for putting into an array using `array get`.

The list elements are **path** followed by the znode path, **zk** followed by the name of the zookeepertcl zookeeper command that was used to create the watch, **type** followed by the zookeeper type and **state** followed by the zookeeper state.

State will be one of **closed**, **connecting**, **associating**, **connected**, **expired**, **auth_failed** and **unknown**.

Type will be one of **created**, **deleted**, **changed**, **child**, **session**, **not_watching** or **unknown**.

Stat Structure
---
When requesting status using **-stat**, the specified array will be filled with the following elements:
* czxid - the zxid of the change that caused this znode to be created.
* mzxid - the zxid of the change that last modified this znode.
* ctime - the time in milliseconds from epoch when this znode was created.
* mtime - the time in milliseconds from epoch when this znode was last modified.
* version - the number of changes to the data of this znode.
* cversion - the number of changes to the children of this znode.
* aversion - the number of changes to the ACL of this znode.
* ephemeralOwner - the session id of the owner of this znode if the znode is an ephemeral node. If it is not an ephemeral node, it will be zero.
* dataLength - the length of the data field of this znode.
* numChildren - the number of children of this znode.

Errata
---

* ACLs are not currently supported.

FlightAware
---
FlightAware has released over a dozen applications  (under the free and liberal BSD license) into the open source community. FlightAware's repositories are available on GitHub for public use, discussion, bug reports, and contribution. Read more at https://flightaware.com/about/code/

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

zookeeper::zookeeper version returns C client version, like 3.4.6.

set zk [zookeeper init #auto localhost:2181 50000]
zookeeper init zk localhost:2181 50000

set realID [$zk create /k -value woof -ephemeral]
$zk get $realID 0 z

This creates an ephemeral (for the life of the process) node on zookeeper.  You have to use the ID returned because it may be different from the one you pass in.  Like /k becomes /k00000000.  (I think this change only occurs if -ephemeral or -sequence is used.)

$zk create path ?-value value? ?-ephemeral? ?-sequence?

Create the path.  Value, if provided, is set as the value at the path else the node's value is left as null.  "-ephemeral" makes the path exist only for the life of the connection from this process in accordance with normal zookeeper behavior.  If "-sequence" is provided, a unique monotonically increasing sequence number is appended to the pathname.

$zk get $path ?-watch code? ?-stat array?

Get the data at node $path.  A watch is set if -watch is specified and code is invoked with an argument of a list of key-value pairs about the watched object.  If -stat is specified, array is the name of an array that is filled with stat data such as version.

$zk exists path ?-watch code? ?-stat arrayName?

Return 1 if the path exists and 0 if it doesn't.  -watch and -stat are the same as for "get" above.

$zk get_children path

Return a list of the child znodes of the given path.

(The C API supports adding a watch with this call but we currently do not.)

$zk set $path $data $version

Set the given path to the given data. Version must match.

Zookeeper supports null data so if we are to support this properly, i.e. out of band, well we need to make providing data optional and set it to null and we already have done that, so we're half way.  But now we have to make a way for "get" to provide an out of band result, like having it return 1 or 0 and store the results into a variable provided through an argument.

$zk delete path version

Delete the path on zookeeper server.  Version must be the right version number in accordance with normal zookeeper rules.

$zk state

This returns the state of the zookeeper session.  It can be CLOSED, CONNECTING, ASSOCIATING, CONNECTED, EXPIRED, or AUTH_FAILED.

$zk recv_timeout

This returns the timeout for the session.  It's only valid if the state is connected.  zookeeper C API docs say the value may change after a server reconnect.

$zk is_unrecoverable

return true if the zookeeper C library says the connection state can't be recovered.

If this returns true then the application must close the zhandle object and try to reconnect.


FlightAware
---
FlightAware has released over a dozen applications  (under the free and liberal BSD license) into the open source community. FlightAware's repositories are available on GitHub for public use, discussion, bug reports, and contribution. Read more at https://flightaware.com/about/code/

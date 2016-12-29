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
```

zookeeper init hosts


FlightAware
---
FlightAware has released over a dozen applications  (under the free and liberal BSD license) into the open source community. FlightAware's repositories are available on GitHub for public use, discussion, bug reports, and contribution. Read more at https://flightaware.com/about/code/

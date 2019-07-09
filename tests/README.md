# `zookeepertcl` Unit Tests

This folder contains unit tests for the `zookeepertcl` library.  It has tests for the Zookeeper API, watch callbacks and some of the `zookeepertcl` Tcl procs created in the `zookeeper` namespace.  Rather than mocking Zookeeper interaction, these unit tests communicate with an actual Zookeeper server / cluster.  Be warned, though, that the unit tests themselves do not setup a Zookeeper server / cluster: this must be done prior to executing the tests.  In addition, these tests impose an additional constraint that running any of the unit tests *must* be done through the `all.tcl` file, which takes some command line arguments: 

- `zkHostString`: defaults to `localhost:2181`, this is the host string used for connecting to Zookeeper.  
- `zkTestRoot`: defaults to `/zktcl_test`, this is the root znode which will contain all the data used by the unit tests.  Prior to running the unit tests this path is deleted and re-created.  Using a root path for any unit test data allows for a quick cleanup of any znodes created during testing.  While the unit tests themselves attempt to delete any nodes they create, in the case of an error this might not happen, so it is useful in that case to have any test data contained under a known root path.
- `zkDebugLevel`: defaults to `none` and controls the debug level output by the library to `stderr`
- `zkTimeout`: defaults to `3000` and sets the millisecond timeout for connecting to Zookeeper
- `zkSyncTimeout`: defaults to `1500` milliseconds, this controls how long the unit tests for watch callbacks will wait before timing out
- `testDebugLevel`: defaults to `0` and controls the Tcl test debug level 
- `testFiles`: defaults to `*.test` and controls which test files to run unit tests from
- `testMatch`: defaults to `*` and controls which test cases in the `testFiles` specified to actually run
- `testSkip`: defaults to `""` and controls which test cases in the `testFiles` specified to skip over
- `testVerbosity`: defaults to `body error` and controls the verbosity of Tcl test

## Examples of Usage

Running all the tests: 

```
tclsh all.tcl -zkHostString "server1.example.com:2181,server2.example.com:2181,server3.example.com"
```

Running only the API tests:

```
tclsh all.tcl -zkHostString "server1.example.com:2181,server2.example.com:2181,server3.example.com" -testFiles api.test
```

Running only the `get` watch tests:

```
tclsh all.tcl -zkHostString "server1.example.com:2181,server2.example.com:2181,server3.example.com" -testFiles watches.test -testMatch "get_*"
```

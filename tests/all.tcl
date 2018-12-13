# all.tcl --
#
# This file contains a top-level script to run all of the Tcl
# tests.  Execute it by invoking "source all.test" when running tcltest
# in this directory.
#
# Copyright (c) 1998-1999 by Scriptics Corporation.
# Copyright (c) 2000 by Ajuba Solutions
#
# See the file "license.terms" for information on usage and redistribution
# of this file, and for a DISCLAIMER OF ALL WARRANTIES.

package require cmdline
package require zookeeper

proc connect_to_zookeeper {} {
    zookeeper::zookeeper debug_level $::params(zkDebugLevel)
    zookeeper::zookeeper init zk $::params(zkHostString) $::params(zkTimeout) -async init_callback
    after $::params(zkTimeout) {set ::connected 0} 
    vwait ::connected

    if {!$::connected} {
	puts stderr "Could not connect to $::params(zkHostString)"
	exit 1
    }
}

proc init_callback {iDict} {
    set ::connected 1
}

proc main {argv} {
    set usage ": $::argv0 ?options?"
    set options {
	{skip.arg "" "List of patterns to determine whether a test should be skipped"}
	{testDebugLevel.arg 0 "Test debug level"}
	{testFiles.arg "*.test" "List of patterns to determine what test files to evaluate"}
	{testMatch.arg "*" "List of patterns to determine which test cases should be run"}
	{testVerbosity.arg "body error" "Type of output verbosity"}
	{zkDebugLevel.arg "none" "debug_level to set in the zookeepertcl library during the tests"}
	{zkHostString.arg "localhost:2181" "Zookeeper connection string used for running tests"}
	{zkSyncTimeout.arg 1500 "Async callback timeout to ensure async tests cannot hang indefinitely"}
	{zkTimeout.arg 3000 "Connection timeout in milliseconds"}
    } 

    try {
	array set ::params [::cmdline::getoptions argv $options $usage]
    } on error {result options} {
	puts stderr $result
	exit 1
    }

    set tcltestVersion [package require tcltest]
    namespace import -force tcltest::*

    tcltest::configure -singleproc true; # use a singleproc so zk object can be shared
    tcltest::configure -debug $::params(testDebugLevel)
    tcltest::configure -verbose $::params(testVerbosity)
    tcltest::configure -file $::params(testFiles)
    tcltest::configure -match $::params(testMatch)
    tcltest::configure -skip $::params(skip)

    connect_to_zookeeper

    tcltest::testsDirectory [file dir [info script]]
    tcltest::runAllTests
}

main $argv

# vim: set ts=8 sw=4 sts=4 noet :

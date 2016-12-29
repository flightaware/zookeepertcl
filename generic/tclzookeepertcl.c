/*
 * zookeeper_Init and zookeeper_SafeInit
 *
 * Copyright (C) 2016 - 2017 FlightAware
 *
 * Freely redistributable under the Berkeley copyright.  See license.terms
 * for details.
 */

#include <tcl.h>
#include "zookeepertcl.h"

#undef TCL_STORAGE_CLASS
#define TCL_STORAGE_CLASS DLLEXPORT


/*
 *----------------------------------------------------------------------
 *
 * Zookeeper_Init --
 *
 *	Initialize the zookeeper extension.  The string "zookeeper"
 *      in the function name must match the PACKAGE declaration at the top of
 *	configure.in.
 *
 * Results:
 *	A standard Tcl result
 *
 * Side effects:
 *	One new command "zookeeper" is added to the Tcl interpreter.
 *
 *----------------------------------------------------------------------
 */

EXTERN int
Zookeeper_Init(Tcl_Interp *interp)
{
    Tcl_Namespace *namespace;
    /*
     * This may work with 8.0, but we are using strictly stubs here,
     * which requires 8.1.
     */
    if (Tcl_InitStubs(interp, "8.1", 0) == NULL) {
		return TCL_ERROR;
    }

    if (Tcl_PkgRequire(interp, "Tcl", "8.1", 0) == NULL) {
		return TCL_ERROR;
    }

    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
		return TCL_ERROR;
    }

    namespace = Tcl_CreateNamespace (interp, "::zookeeper", NULL, NULL);

    /* Create the create command  */
    Tcl_CreateObjCommand(interp, "::zookeeper::zookeeper", (Tcl_ObjCmdProc *) zookeepertcl_zookeeperObjCmd, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_Export (interp, namespace, "*", 0);

    return TCL_OK;
}


/*
 *----------------------------------------------------------------------
 *
 * Zookeeper_SafeInit --
 *
 *	Initialize the kafkatcl in a safe interpreter.
 *
 * Results:
 *	A standard Tcl result
 *
 * Side effects:
 *	One new command "kafkatcl" is added to the Tcl interpreter.
 *
 *----------------------------------------------------------------------
 */

EXTERN int
Zookeeper_SafeInit(Tcl_Interp *interp)
{
    /*
     * can this work safely?  I don't know...
     */
    return TCL_ERROR;
}

/* vim: set ts=4 sw=4 sts=4 noet : */

/* -*- mode: c; tab-width: 4; indent-tabs-mode: t -*- */

/*
 * zookeepertcl - Tcl interface to Apache Zookeeper
 *
 * Copyright (C) 2016 - 2017 FlightAware LLC
 *
 * freely redistributable under the Berkeley license
 */

#include "zookeepertcl.h"
#include <assert.h>
#include <stdlib.h>

/*
 *--------------------------------------------------------------
 *
 * zookeeper_state_to_string -- given a zookeeper
 *   state code return a string corresponding
 *
 * Results:
 *      returns a pointer to a const char *
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */
const char *zookeeper_state_to_string (int state)
{
	if (state == 0)
		return "CLOSED_STATE";
	if (state == ZOO_CONNECTING_STATE)
		return "CONNECTING_STATE";
	if (state == ZOO_ASSOCIATING_STATE)
		return "ASSOCIATING_STATE";
	if (state == ZOO_CONNECTED_STATE)
		return "CONNECTED_STATE";
	if (state == ZOO_EXPIRED_SESSION_STATE)
		return "EXPIRED_SESSION_STATE";
	if (state == ZOO_AUTH_FAILED_STATE)
		return "AUTH_FAILED_STATE";

	return "INVALID_STATE";
}

/*
 *--------------------------------------------------------------
 *
 * zookeeper_type_to_string -- given a zookeeper type
 *   return a corresponding string
 *
 * Results:
 *      returns a pointer to a const char *
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */
const char *zookeeper_type_to_string (int state)
{
	if (state == ZOO_CREATED_EVENT)
		return "CREATED_EVENT";
	if (state == ZOO_DELETED_EVENT)
		return "DELETED_EVENT";
	if (state == ZOO_CHANGED_EVENT)
		return "CHANGED_EVENT";
	if (state == ZOO_CHILD_EVENT)
		return "CHILD_EVENT";
	if (state == ZOO_SESSION_EVENT)
		return "SESSION_EVENT";
	if (state == ZOO_NOTWATCHING_EVENT)
		return "NOTWATCHING_EVENT";

	return "UNKNOWN_EVENT_TYPE";
}


/*
 *--------------------------------------------------------------
 *
 * zookeeper_error_to_code_string -- given a zookeeper error
 *   return a corresponding string
 *
 * Results:
 *      returns a pointer to a const char *
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */
const char *zookeeper_error_to_code_string (int status)
{
	switch (status) {
		case ZOK:
			return "ZOK";

		case ZSYSTEMERROR:
			return "ZSYSTEMERROR";

		case ZRUNTIMEINCONSISTENCY:
			return "ZRUNTIMEINCONSISTENCY";

		case ZDATAINCONSISTENCY:
			return "ZDATAINCONSISTENCY";

		case ZCONNECTIONLOSS:
			return "ZCONNECTIONLOSS";

		case ZMARSHALLINGERROR:
			return "ZMARSHALLINGERROR";

		case ZUNIMPLEMENTED:
			return "ZUNIMPLEMENTED";

		case ZOPERATIONTIMEOUT:
			return "ZOPERATIONTIMEOUT";

		case ZBADARGUMENTS:
			return "ZBADARGUMENTS";

		case ZINVALIDSTATE:
			return "ZINVALIDSTATE";

		case ZAPIERROR:
			return "ZAPIERROR";

		case ZNONODE:
			return "ZNONODE";

		case ZNOAUTH:
			return "ZNOAUTH";

		case ZBADVERSION:
			return "ZBADVERSION";

		case ZNOCHILDRENFOREPHEMERALS:
			return "ZNOCHILDRENFOREPHEMERALS";

		case ZNODEEXISTS:
			return "ZNODEEXISTS";

		case ZNOTEMPTY:
			return "ZNOTEMPTY";

		case ZSESSIONEXPIRED:
			return "ZSESSIONEXPIRED";

		case ZINVALIDCALLBACK:
			return "ZINVALIDCALLBACK";

		case ZINVALIDACL:
			return "ZINVALIDACL";

		case ZAUTHFAILED:
			return "ZAUTHFAILED";

		case ZCLOSING:
			return "ZCLOSING";

		case ZNOTHING:
			return "ZNOTHING";

		case ZSESSIONMOVED:
			return "ZSESSIONMOVED";

		default:
			return "ZUNKNOWN";
	}
}

/*
 *--------------------------------------------------------------
 *
 * zookeeper_set_tcl_return_code -- given a zookeeper status
 *   return TCL_OK if it's ZOK else set an error result and
 *   return TCL_ERROR
 *
 * Results:
 *      returns TCL_OK or TCL_ERROR
 *
 * Side effects:
 *      May set an error message into the Tcl result and
 *      set errorCode.
 *
 *--------------------------------------------------------------
 */
int
zookeeper_set_tcl_return_code (int status) {
	if (status == ZOK) {
		return TCL_OK;
	}

	// NB this needs to be spruced up to set errorCode and a
	// better error message and stuff
	Tcl_SetObjResult (interp, Tcl_NewStringObj (stateString, -1));
	return TCL_ERROR;
}

/*
 *--------------------------------------------------------------
 *
 * zookeepertcl_zookeeperObjectDelete -- command deletion callback routine.
 *
 * Results:
 *      ...destroys the zookeeper object.
 *      ...frees memory.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */
void
zookeepertcl_zookeeperObjectDelete (ClientData clientData)
{
    zookeepertcl_objectClientData *zo = (zookeepertcl_objectClientData *)clientData;

    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	int status = zoopeeker_close (zo->zh);
	if (status != ZOK) {
		char *errorString = zookeeper_error_to_string (status);
	}

	// destroy stuff here
    ckfree((char *)clientData);
}

/*
 *----------------------------------------------------------------------
 *
 * zookeeper_EventSetupProc --
 *    This routine is a required argument to Tcl_CreateEventSource
 *
 *    Since we need to poll librdkafka to get events to fire, let's
 *    make sure we get called periodically
 *
 *
 * Results:
 *    Our polling routine will get called periodically.
 *
 *----------------------------------------------------------------------
 */
void
zookeeper_EventSetupProc (ClientData clientData, int flags) {
	Tcl_Time time = {0, 100000};

	Tcl_SetMaxBlockTime (&time);
}

/*
 *----------------------------------------------------------------------
 *
 * zookeeper_EventCheckProc --
 *
 *    This is a function we pass to Tcl_CreateEventSource that is
 *    invoked to see if any events have occurred and to queue them.
 *
 *    rdkafkalib requires that we invoke a poll function to trigger
 *    the kafka-provided callbacks to be invoked.  So we do that.
 *
 * Results:
 *    The program compiles.
 *
 *----------------------------------------------------------------------
 */
void
zookeeper_EventCheckProc (ClientData clientData, int flags) {
    zookeepertcl_objectClientData *zo = (zookeepertcl_objectClientData *)clientData;

}

/*
 *----------------------------------------------------------------------
 *
 * zookeepertcl_zookeeperObjectObjCmd --
 *
 *      perform methods of a zookeeper object
 *
 * Results:
 *      A standard Tcl result.
 *
 *
 *----------------------------------------------------------------------
 */

    /* ARGSUSED */
int
zookeepertcl_zookeeperObjectObjCmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
    zookeepertcl_objectClientData *zo = (zookeepertcl_objectClientData *)clientData;
	ZOOAPI zhandle_t *zh = zo->zh;

    static CONST char *options[] = {
        "exists",
        "delete",
        "state",
        "recv_timeout",
        "is_unrecoverable",
        NULL
    };

    enum options {
		OPT_EXISTS,
		OPT_DELETE,
        OPT_STATE,
		OPT_RECV_TIMEOUT,
		OPT_IS_UNRECOVERABLE
    };

    // basic command line processing
    if (objc < 2) {
        Tcl_WrongNumArgs (interp, 1, objv, "subcommand ?args?");
        return TCL_ERROR;
    }

    // argument must be one of the subOptions defined above
    if (Tcl_GetIndexFromObj (interp, objv[1], options, "option",
        TCL_EXACT, &optIndex) != TCL_OK) {
        return TCL_ERROR;
    }

    switch ((enum options) optIndex) {
		case OPT_EXISTS:
		{
			char *path;
			int watch = 0;
			struct Stat stat;

			if (objc != 4) {
				Tcl_WrongNumArgs (interp, 2, objv, "path watch");
				return TCL_ERROR;
			}

			path = Tcl_GetString (objv[2]);

			if (Tcl_GetBooleanFromObj (interp, objv[3], &watch) == TCL_ERROR) {
				return TCL_ERROR;
			}

			int status = zoo_exists (zh, path, watch, &stat);
			return zookeeper_set_tcl_return_code (status);
		}

		case OPT_DELETE:
		{
			char *path;
			int version = 0;

			if (objc != 4) {
				Tcl_WrongNumArgs (interp, 2, objv, "path version");
				return TCL_ERROR;
			}

			path = Tcl_GetString (objv[2]);

			if (Tcl_GetIntFromObj (interp, objv[3], &version) == TCL_ERROR) {
				return TCL_ERROR;
			}

			int status = zoo_delete (zh, path, version);
			return zookeeper_set_tcl_return_code (status);
		}

		case OPT_STATE:
		{
			if (objc != 2) {
				Tcl_WrongNumArgs (interp, 2, objv, "");
				return TCL_ERROR;
			}

			int state = zoo_state (zh);
			const char *stateString = zookeeper_state_to_string (state);
			Tcl_SetObjResult (interp, Tcl_NewStringObj (stateString, -1));
			break;
		}

		case OPT_RECV_TIMEOUT:
		{
			if (objc != 2) {
				Tcl_WrongNumArgs (interp, 2, objv, "");
				return TCL_ERROR;
			}

			Tcl_SetObjResult (interp, Tcl_NewIntObj (zoo_recv_timeout (zh)));
			break;
		}

		case OPT_IS_UNRECOVERABLE:
		{
			Tcl_SetObjResult (interp, Tcl_NewBooleanObj (is_recoverable (zh) == ZINVALIDSTATE));
			break;
		}
	}

    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * zookeepertcl_zookeeperObjCmd --
 *
 *      Create a zookeeper object...
 *
 *      zookeeper create
 *
 * The created object something something
 *
 * Results:
 *      A standard Tcl result.
 *
 *
 *----------------------------------------------------------------------
 */

    /* ARGSUSED */
int
zookeepertcl_zookeeperObjCmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
    zookeepertcl_objectClientData *zo;
    int                 optIndex;
    char               *cmdName;

    static CONST char *options[] = {
        "init",
        "version",
        NULL
    };

    enum options {
        OPT_INIT,
		OPT_VERSION
    };

    // basic command line processing
    if (objc < 2) {
        Tcl_WrongNumArgs (interp, 1, objv, "subcommand ?args?");
        return TCL_ERROR;
    }

    // argument must be one of the subOptions defined above
    if (Tcl_GetIndexFromObj (interp, objv[1], options, "option",
        TCL_EXACT, &optIndex) != TCL_OK) {
        return TCL_ERROR;
    }

    switch ((enum options) optIndex) {
		case OPT_VERSION:
		{
			char format[120];

			if (objc != 2) {
				Tcl_WrongNumArgs (interp, 2, objv, "");
				return TCL_ERROR;
			}

			snprintf (format, sizeof(format), "%d.%d.%d", ZOO_MAJOR_VERSION, ZOO_MINOR_VERSION, ZOO_PATCH_VERSION);
			Tcl_SetObjResult (interp, Tcl_NewStringObj (format, -1));
			return TCL_OK;
		}

		case OPT_INIT: {
		    int timeout;

			if (objc != 5) {
				Tcl_WrongNumArgs (interp, 2, objv, "cmdName hosts timeout");
				return TCL_ERROR;
			}

			if (Tcl_GetIntFromObj (interp, objv[4], &timeout) == TCL_ERROR) {
				return TCL_ERROR;
			}

			// allocate one of our kafka client data objects for Tcl and configure it
			zo = (zookeepertcl_objectClientData *)ckalloc (sizeof (zookeepertcl_objectClientData));
			zo->zookeeper_object_magic = ZOOKEEPER_OBJECT_MAGIC;
			zo->interp = interp;

			char *cmdName = Tcl_GetString (objv[2]);
			char *hosts = Tcl_GetString (objv[3]);

			zo->zh = zookeeper_init (hosts, watcher, timeout, NULL, zo, 0);


			// if cmdName is #auto, generate a unique name for the object
			int autoGeneratedName = 0;
			if (strcmp (cmdName, "#auto") == 0) {
				static unsigned long nextAutoCounter = 0;
				int    baseNameLength;

#define OBJECT_STRING_FORMAT "zookeeper%lu"
				baseNameLength = snprintf (NULL, 0, OBJECT_STRING_FORMAT, nextAutoCounter) + 1;
				cmdName = ckalloc (baseNameLength);
				snprintf (cmdName, baseNameLength, OBJECT_STRING_FORMAT, nextAutoCounter++);
				autoGeneratedName = 1;
			}

			// create a Tcl command to interface to zookeeper
			zo->cmdToken = Tcl_CreateObjCommand (interp, cmdName, zookeepertcl_zookeeperObjectObjCmd, zo, zookeepertcl_zookeeperObjectDelete);
			Tcl_SetObjResult (interp, Tcl_NewStringObj (cmdName, -1));
			if (autoGeneratedName == 1) {
				ckfree(cmdName);
			}
			break;
		}

	}

    return TCL_OK;
}

/* vim: set ts=4 sw=4 sts=4 noet : */




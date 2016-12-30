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
 * zookeepertcl_state_to_string -- given a zookeeper
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
const char *zookeepertcl_state_to_string (int state)
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
 * zookeepertcl_type_to_string -- given a zookeeper type
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
const char *zookeepertcl_type_to_string (int state)
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
 * zookeepertcl_error_to_code_string -- given a zookeeper error
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
const char *zookeepertcl_error_to_code_string (int status)
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
 * zookeepertcl_stat_to_array -- given an interp, array name
 *   and zookeeper Stat struct, set elements in the array
 *   with values corresponding to the Stat struct
 *
 * Results:
 *      returns a pointer to a const char *
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */
int zookeepertcl_stat_to_array (Tcl_Interp *interp, char *arrayName, struct Stat *stat)
{
	if (Tcl_SetVar2Ex (interp, arrayName, "czxid", Tcl_NewLongObj (stat->czxid), TCL_LEAVE_ERR_MSG) == NULL) {
		return TCL_ERROR;
	}

	if (Tcl_SetVar2Ex (interp, arrayName, "mzxid", Tcl_NewLongObj (stat->mzxid), TCL_LEAVE_ERR_MSG) == NULL) {
		return TCL_ERROR;
	}

	if (Tcl_SetVar2Ex (interp, arrayName, "ctime", Tcl_NewLongObj (stat->ctime), TCL_LEAVE_ERR_MSG) == NULL) {
		return TCL_ERROR;
	}

	if (Tcl_SetVar2Ex (interp, arrayName, "mtime", Tcl_NewLongObj (stat->mtime), TCL_LEAVE_ERR_MSG) == NULL) {
		return TCL_ERROR;
	}

	if (Tcl_SetVar2Ex (interp, arrayName, "version", Tcl_NewIntObj (stat->version), TCL_LEAVE_ERR_MSG) == NULL) {
		return TCL_ERROR;
	}

	if (Tcl_SetVar2Ex (interp, arrayName, "cversion", Tcl_NewIntObj (stat->cversion), TCL_LEAVE_ERR_MSG) == NULL) {
		return TCL_ERROR;
	}

	if (Tcl_SetVar2Ex (interp, arrayName, "aversion", Tcl_NewIntObj (stat->aversion), TCL_LEAVE_ERR_MSG) == NULL) {
		return TCL_ERROR;
	}

	if (Tcl_SetVar2Ex (interp, arrayName, "ephemeralOwner", Tcl_NewLongObj (stat->ephemeralOwner), TCL_LEAVE_ERR_MSG) == NULL) {
		return TCL_ERROR;
	}

	if (Tcl_SetVar2Ex (interp, arrayName, "dataLength", Tcl_NewIntObj (stat->dataLength), TCL_LEAVE_ERR_MSG) == NULL) {
		return TCL_ERROR;
	}

	if (Tcl_SetVar2Ex (interp, arrayName, "numChildren", Tcl_NewIntObj (stat->numChildren), TCL_LEAVE_ERR_MSG) == NULL) {
		return TCL_ERROR;
	}

	if (Tcl_SetVar2Ex (interp, arrayName, "pzxid", Tcl_NewLongObj (stat->pzxid), TCL_LEAVE_ERR_MSG) == NULL) {
		return TCL_ERROR;
	}

	return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * zookeepertcl_set_tcl_return_code -- given a zookeeper status
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
zookeepertcl_set_tcl_return_code (Tcl_Interp *interp, int status) {
	if (status == ZOK) {
		return TCL_OK;
	}
	const char *stateString = zookeepertcl_error_to_code_string (status);

	// NB this needs to be spruced up to set errorCode and a
	// better error message and stuff
	Tcl_SetObjResult (interp, Tcl_NewStringObj (stateString, -1));
	return TCL_ERROR;
}

void zookeepertcl_watcher (zhandle_t *zh, int type, int state, const char *path, void* context)
{
    zookeepertcl_objectClientData *zo = (zookeepertcl_objectClientData *)context;
	const char *typeString = zookeepertcl_type_to_string (type);
	const char *stateString = zookeepertcl_state_to_string (state);

	printf("zookeepertcl_watcher called type '%s' state '%s' path '%s'\n", typeString, stateString, path);
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

	zookeeper_close (zo->zh);
    ckfree((char *)clientData);
}

/*
 *----------------------------------------------------------------------
 *
 * zookeepertcl_EventSetupProc --
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
zookeepertcl_EventSetupProc (ClientData clientData, int flags) {
	Tcl_Time time = {0, 100000};

	Tcl_SetMaxBlockTime (&time);
}

/*
 *----------------------------------------------------------------------
 *
 * zookeepertcl_EventCheckProc --
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
zookeepertcl_EventCheckProc (ClientData clientData, int flags) {
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
	int optIndex;
	struct Stat stat;

    static CONST char *options[] = {
        "get",
        "get_children",
		"set",
        "create",
        "exists",
        "delete",
        "state",
        "recv_timeout",
        "is_unrecoverable",
        NULL
    };

    enum options {
		OPT_GET,
		OPT_GET_CHILDREN,
		OPT_SET,
		OPT_CREATE,
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
		case OPT_SET:
		{
			char *path;
			char *buffer;
			int bufferLen = 0;
			int version = 0;

			if (objc != 5) {
				Tcl_WrongNumArgs (interp, 2, objv, "path data version");
				return TCL_ERROR;
			}

			path = Tcl_GetString (objv[2]);
			buffer = Tcl_GetStringFromObj (objv[3], &bufferLen);

			if (Tcl_GetIntFromObj (interp, objv[4], &version) == TCL_ERROR) {
				return TCL_ERROR;
			}

			int status = zoo_set (zh, path, buffer, bufferLen, version);
			return zookeepertcl_set_tcl_return_code (interp, status);
		}

		case OPT_GET_CHILDREN:
		{
			char *path;
			int watch = 0;
			struct String_vector strings;
			int i;

			if (objc != 4) {
				Tcl_WrongNumArgs (interp, 2, objv, "path watch");
				return TCL_ERROR;
			}

			path = Tcl_GetString (objv[2]);
			if (Tcl_GetBooleanFromObj (interp, objv[3], &watch) == TCL_ERROR) {
				return TCL_ERROR;
			}

			int status = zoo_get_children (zh, path, watch, &strings);
			if (status != ZOK) {
				return zookeepertcl_set_tcl_return_code (interp, status);
			}

			for (i = 0; i < strings.count; i++) {
				if (Tcl_ListObjAppendElement (interp, Tcl_GetObjResult (interp), Tcl_NewStringObj (strings.data[i], -1)) == TCL_ERROR) {
					return TCL_ERROR;
				}
			}
			return TCL_OK;
		}

		case OPT_GET:
		{
			char *path;
			int watch = 0;
			char buffer[1024*1024];
			int bufferLen = sizeof(buffer);

			if (objc != 5) {
				Tcl_WrongNumArgs (interp, 2, objv, "path watch statArrayName");
				return TCL_ERROR;
			}

			path = Tcl_GetString (objv[2]);
			if (Tcl_GetBooleanFromObj (interp, objv[3], &watch) == TCL_ERROR) {
				return TCL_ERROR;
			}
			char *statArray = Tcl_GetString (objv[4]);

			int status = zoo_get (zh, path, watch, buffer, &bufferLen, NULL);
			if (status == ZOK) {
				Tcl_SetObjResult (interp, Tcl_NewStringObj (buffer, bufferLen));
			}

			if (zookeepertcl_stat_to_array (interp, statArray, &stat) == TCL_ERROR) {
				return TCL_ERROR;
			}
			return zookeepertcl_set_tcl_return_code (interp, status);
		}


		case OPT_CREATE:
		{
			static CONST char *subOptions[] = {
				"-value",
				"-ephemeral",
				"-sequence",
				NULL
			};

			enum subOptions {
				SUBOPT_VALUE,
				SUBOPT_EPHEMERAL,
				SUBOPT_SEQUENCE
			};

			char *path;
			int valueLen = -1;
			char *value = NULL;
			int flags = 0;

			if (objc < 3)  {
				Tcl_WrongNumArgs (interp, 2, objv, "path ?-value value? ?-ephemeral? ?-sequence?");
				return TCL_ERROR;
			}
			path = Tcl_GetString (objv[2]);
			int i;
			int suboptIndex = 0;

			for (i = 3; i < objc; i++) {
				if (Tcl_GetIndexFromObj (interp, objv[i], subOptions, "suboption",
					TCL_EXACT, &suboptIndex) != TCL_OK) {
					return TCL_ERROR;
				}

				switch ((enum subOptions) suboptIndex) {
					case SUBOPT_VALUE:
					{
						if (i + 1 >= objc) {
							Tcl_WrongNumArgs (interp, 2, objv, "-value value");
							return TCL_ERROR;
						}
						value = Tcl_GetStringFromObj (objv[i++], &valueLen);
					}

					case SUBOPT_EPHEMERAL:
					{
						flags |= ZOO_EPHEMERAL;
					}

					case SUBOPT_SEQUENCE:
					{
						flags |= ZOO_SEQUENCE;
					}
				}
			}

			char returnPathBuf[1024*1024];

			int status = zoo_create (zh, path, value, valueLen, NULL, flags, returnPathBuf, sizeof(returnPathBuf));
			if (status == ZOK) {
				Tcl_SetObjResult (interp, Tcl_NewStringObj (returnPathBuf, -1));
			}
			return zookeepertcl_set_tcl_return_code (interp, status);
		}

		case OPT_EXISTS:
		{
			char *path;
			int watch = 0;
			struct Stat stat;

			if (objc != 5) {
				Tcl_WrongNumArgs (interp, 2, objv, "path watch statArrayName");
				return TCL_ERROR;
			}

			path = Tcl_GetString (objv[2]);
			if (Tcl_GetBooleanFromObj (interp, objv[3], &watch) == TCL_ERROR) {
				return TCL_ERROR;
			}
			char *statArray = Tcl_GetString (objv[4]);

			int status = zoo_exists (zh, path, watch, &stat);
			if (status == ZOK) {
				if (zookeepertcl_stat_to_array (interp, statArray, &stat) == TCL_ERROR) {
					return TCL_ERROR;
				}
			}
			return zookeepertcl_set_tcl_return_code (interp, status);
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
			return zookeepertcl_set_tcl_return_code (interp, status);
		}

		case OPT_STATE:
		{
			if (objc != 2) {
				Tcl_WrongNumArgs (interp, 2, objv, "");
				return TCL_ERROR;
			}

			int state = zoo_state (zh);
			const char *stateString = zookeepertcl_state_to_string (state);
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
			Tcl_SetObjResult (interp, Tcl_NewBooleanObj (is_unrecoverable (zh) == ZINVALIDSTATE));
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
        "debug_level",
        NULL
    };

    enum options {
        OPT_INIT,
		OPT_VERSION,
		OPT_DEBUG_LEVEL
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

			zo->zh = zookeeper_init (hosts, zookeepertcl_watcher, timeout, NULL, zo, 0);
			zoo_set_context (zo->zh, (void *)zo);

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

		case OPT_DEBUG_LEVEL:
		{
			int zooLogLevel = 0;
			int subOption = 0;
			static CONST char *subOptions[] = {
				"error",
				"warn",
				"info",
				"debug",
				NULL
			};

			enum subOptions {
				SUBOPT_ERROR,
				SUBOPT_WARN,
				SUBOPT_INFO,
				SUBOPT_DEBUG
			};


			if (objc != 3) {
				Tcl_WrongNumArgs (interp, 2, objv, "level");
				return TCL_ERROR;
			}

			if (Tcl_GetIndexFromObj (interp, objv[2], subOptions, "logLevel", TCL_EXACT, &subOption) != TCL_OK) {
				return TCL_ERROR;
			}
			switch ((enum subOptions) subOption) {
				case SUBOPT_ERROR:
					zooLogLevel = ZOO_LOG_LEVEL_ERROR;
					break;

				case SUBOPT_WARN:
					zooLogLevel = ZOO_LOG_LEVEL_WARN;
					break;

				case SUBOPT_INFO:
					zooLogLevel = ZOO_LOG_LEVEL_INFO;
					break;

				case SUBOPT_DEBUG:
					zooLogLevel = ZOO_LOG_LEVEL_DEBUG;
					break;
			}

			zoo_set_debug_level (zooLogLevel);
		}

	}

    return TCL_OK;
}

/* vim: set ts=4 sw=4 sts=4 noet : */




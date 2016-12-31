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

int
zookeepertcl_EventProc (Tcl_Event *tevPtr, int flags);

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
		return "closed";
	if (state == ZOO_CONNECTING_STATE)
		return "connecting";
	if (state == ZOO_ASSOCIATING_STATE)
		return "associating";
	if (state == ZOO_CONNECTED_STATE)
		return "connected";
	if (state == ZOO_EXPIRED_SESSION_STATE)
		return "expired";
	if (state == ZOO_AUTH_FAILED_STATE)
		return "auth_failed";

	return "invalid";
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
		return "created";
	if (state == ZOO_DELETED_EVENT)
		return "deleted";
	if (state == ZOO_CHANGED_EVENT)
		return "changed";
	if (state == ZOO_CHILD_EVENT)
		return "child";
	if (state == ZOO_SESSION_EVENT)
		return "session";
	if (state == ZOO_NOTWATCHING_EVENT)
		return "not_watching";

	return "invalid";
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
	Tcl_SetErrorCode(interp, "ZOOKEEPER", stateString, (char *) NULL);
	return TCL_ERROR;
}

/*
 *--------------------------------------------------------------
 *
 * zookeepertcl_watcher -- watcher callback function
 *
 * we can't call Tcl directly here because this has occurred
 * asynchronously to whatever the interpreter is doing, so
 * we queue an event to the interpreter instead.
 *
 *--------------------------------------------------------------
 */
void zookeepertcl_watcher (zhandle_t *zh, int type, int state, const char *path, void* context)
{
	zookeepertcl_callbackEvent *evPtr;

	evPtr = ckalloc (sizeof (zookeepertcl_callbackEvent));
	evPtr->event.proc = zookeepertcl_EventProc;

    evPtr->zo = (zookeepertcl_objectClientData *)zoo_get_context (zh);
	evPtr->zookeeperType = type;
	evPtr->zookeeperState = state;
	evPtr->path = path;
	evPtr->commandObj = (Tcl_Obj *)context;

	// Tcl_ThreadQueueEvent (evPtr->zo->threadId, (Tcl_Event *)evPtr, TCL_QUEUE_TAIL);
	Tcl_QueueEvent ((Tcl_Event *)evPtr, TCL_QUEUE_TAIL);

	printf("zookeepertcl_watcher invoked type '%s' state '%s' path '%s'; event queued\n", zookeepertcl_type_to_string (type), zookeepertcl_state_to_string (state), path);
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
 *----------------------------------------------------------------------
 */
void
zookeepertcl_EventCheckProc (ClientData clientData, int flags) {
    // zookeepertcl_objectClientData *zo = (zookeepertcl_objectClientData *)clientData;

}

/*
 *----------------------------------------------------------------------
 *
 * zookeepertcl_EventProc --
 *
 *    this routine is called by the Tcl event handler to process
 *    callbacks we have gotten from zookeeper
 *
 *    in our event structure we have the zookeeper type and state,
 *    the path and the command.
 *
 * Results:
 *    Returns 1 to say we handled the event and the dispatcher can delete it.
 *
 *----------------------------------------------------------------------
 */
int
zookeepertcl_EventProc (Tcl_Event *tevPtr, int flags) {
	zookeepertcl_callbackEvent *evPtr = (zookeepertcl_callbackEvent *)tevPtr;
	zookeepertcl_objectClientData *zo = evPtr->zo;
	Tcl_Interp *interp = zo->interp;
	int tclReturnCode;

	int callbackListObjc;
	Tcl_Obj **callbackListObjv;

	int evalObjc;
	Tcl_Obj **evalObjv;

	printf("zookeepertcl_EventProc invoked\n");

	// crack the command object.  it may be a list.
	if (Tcl_ListObjGetElements (interp, evPtr->commandObj, &callbackListObjc, &callbackListObjv) == TCL_ERROR) {
		Tcl_BackgroundError (interp);
		return 1;
	}

	// construct callback argument as a list

#define ZOOKEEPERTCL_CALLBACK_LISTCOUNT 8

	Tcl_Obj *listObjv[ZOOKEEPERTCL_CALLBACK_LISTCOUNT];

	listObjv[0] = Tcl_NewStringObj ("path", -1);
	listObjv[1] = Tcl_NewStringObj (evPtr->path, -1);

	Tcl_Obj *commandObj = NULL;
	listObjv[2] = Tcl_NewStringObj ("command", -1);
	Tcl_GetCommandFullName (interp, zo->cmdToken, commandObj);
	listObjv[3] = commandObj;

	listObjv[4] = Tcl_NewStringObj ("type", -1);
	listObjv[5] = Tcl_NewStringObj (zookeepertcl_type_to_string (evPtr->zookeeperType), -1);

	listObjv[6] = Tcl_NewStringObj ("state", -1);
	listObjv[7] = Tcl_NewStringObj (zookeepertcl_state_to_string (evPtr->zookeeperState), -1);

	Tcl_Obj *listObj = Tcl_NewListObj (ZOOKEEPERTCL_CALLBACK_LISTCOUNT, listObjv);

	// construct a new list with the command containing as many elements
	// as it needs and the argument list as its final argument

	evalObjc = callbackListObjc + 1;
	evalObjv = (Tcl_Obj **)ckalloc (sizeof (Tcl_Obj *) * evalObjc);

	int i;

	for (i = 0; i < callbackListObjc; i++) {
		evalObjv[i] = callbackListObjv[i];
		Tcl_IncrRefCount (evalObjv[i]);
	}

	evalObjv[evalObjc - 1] = listObj;
	Tcl_IncrRefCount (listObj);

	tclReturnCode = Tcl_EvalObjv (interp, evalObjc, evalObjv, (TCL_EVAL_GLOBAL|TCL_EVAL_DIRECT));

	// if we got a Tcl error, since we initiated the event, it doesn't
	// have anything to traceback further from here to, we must initiate
	// a background error, which will generally cause the bgerror proc
	// to get invoked
	if (tclReturnCode == TCL_ERROR) {
		Tcl_BackgroundError (interp);
	}

	for (i = 0; i < evalObjc; i++) {
		Tcl_DecrRefCount (evalObjv[i]);
	}

	ckfree ((char *)evalObjv);
	return 1;
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
		case OPT_GET:
		{
			static CONST char *subOptions[] = {
				"-watch",
				"-stat",
				NULL
			};

			enum subOptions {
				SUBOPT_WATCH,
				SUBOPT_STAT
			};

			char *path;
			char buffer[1024*1024];
			int bufferLen = sizeof(buffer);
			watcher_fn wfn = NULL;
			struct Stat *stat = NULL;
			struct Stat statBuf;

			if (objc < 3) {
				Tcl_WrongNumArgs (interp, 2, objv, "path ?-watch code? ?-stat statArray?");
				return TCL_ERROR;
			}

			path = Tcl_GetString (objv[2]);

			int i;
			int suboptIndex = 0;
			Tcl_Obj *callbackObj = NULL;
			char *statArray = NULL;

			for (i = 3; i < objc; i++) {
				if (Tcl_GetIndexFromObj (interp, objv[i], subOptions, "suboption",
					TCL_EXACT, &suboptIndex) != TCL_OK) {
					return TCL_ERROR;
				}

				switch ((enum subOptions) suboptIndex) {
					case SUBOPT_WATCH:
					{
						if (i + 1 >= objc) {
							Tcl_WrongNumArgs (interp, 2, objv, "-watch code");
							return TCL_ERROR;
						}
						callbackObj = objv[++i];
						Tcl_IncrRefCount (callbackObj);
						break;
					}

					case SUBOPT_STAT:
					{
						if (i + 1 >= objc) {
							Tcl_WrongNumArgs (interp, 2, objv, "-stat statArray");
							return TCL_ERROR;
						}
						statArray = Tcl_GetString (objv[++i]);
						stat = &statBuf;
						break;
					}
				}
			}

			if (callbackObj != NULL) {
				wfn = zookeepertcl_watcher;
			}

			int status = zoo_wget (zh, path, wfn, (void *)callbackObj, buffer, &bufferLen, stat);
			if (status == ZOK) {
				Tcl_SetObjResult (interp, Tcl_NewStringObj (buffer, bufferLen));
			}

			if (stat != NULL && zookeepertcl_stat_to_array (interp, statArray, stat) == TCL_ERROR) {
				return TCL_ERROR;
			}
			return zookeepertcl_set_tcl_return_code (interp, status);
		}

		case OPT_GET_CHILDREN:
		{
			char *path;
			int watch = 0;
			struct String_vector strings;
			int i;
			watcher_fn wfn = NULL;

			if (objc != 4) {
				Tcl_WrongNumArgs (interp, 2, objv, "path watch");
				return TCL_ERROR;
			}

			path = Tcl_GetString (objv[2]);
			if (Tcl_GetBooleanFromObj (interp, objv[3], &watch) == TCL_ERROR) {
				return TCL_ERROR;
			}
			if (watch) {
				wfn = zookeepertcl_watcher;
			}

			int status = zoo_wget_children (zh, path, wfn, NULL, &strings);
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
						value = Tcl_GetStringFromObj (objv[++i], &valueLen);
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

			int status = zoo_create (zh, path, value, valueLen, &ZOO_OPEN_ACL_UNSAFE, flags, returnPathBuf, sizeof(returnPathBuf));
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
			watcher_fn wfn = NULL;

			if (objc != 5) {
				Tcl_WrongNumArgs (interp, 2, objv, "path watch statArrayName");
				return TCL_ERROR;
			}

			path = Tcl_GetString (objv[2]);
			if (Tcl_GetBooleanFromObj (interp, objv[3], &watch) == TCL_ERROR) {
				return TCL_ERROR;
			}

			if (watch) {
				wfn = zookeepertcl_watcher;
			}
			char *statArray = Tcl_GetString (objv[4]);

			int status = zoo_wexists (zh, path, wfn, NULL, &stat);
			if (status == ZOK || status == ZNONODE) {
				if (zookeepertcl_stat_to_array (interp, statArray, &stat) == TCL_ERROR) {
					return TCL_ERROR;
				}
				Tcl_SetObjResult (interp, Tcl_NewBooleanObj (status == ZOK));
				return TCL_OK;
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
    int                 optIndex;
	zookeepertcl_objectClientData *zo = NULL;

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

			char *cmdName = Tcl_GetString (objv[2]);
			char *hosts = Tcl_GetString (objv[3]);

			zhandle_t *zh = zookeeper_init (hosts, zookeepertcl_watcher, timeout, NULL, zo, 0);
			if (zh == NULL) {
				Tcl_SetObjResult (interp, Tcl_NewStringObj (Tcl_PosixError (interp), -1));
				return TCL_ERROR;
			}

			// allocate one of our kafka client data objects for Tcl and configure it
			zo = (zookeepertcl_objectClientData *)ckalloc (sizeof (zookeepertcl_objectClientData));
			zo->zh = zh;
			zo->zookeeper_object_magic = ZOOKEEPER_OBJECT_MAGIC;
			zo->interp = interp;
			zo->threadId = Tcl_GetCurrentThread ();

			zoo_set_context (zo->zh, (void *)zo);

			Tcl_CreateEventSource (zookeepertcl_EventSetupProc, zookeepertcl_EventCheckProc, (ClientData) zo);

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




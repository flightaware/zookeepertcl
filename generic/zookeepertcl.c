/* -*- mode: c; tab-width: 4; indent-tabs-mode: t -*- */

/*
 * zootcl - Tcl interface to Apache Zookeeper
 *
 * Copyright (C) 2016 - 2017 FlightAware LLC
 *
 * freely redistributable under the Berkeley license
 */

#include "zookeepertcl.h"
#include <assert.h>
#include <stdlib.h>

int
zootcl_EventProc (Tcl_Event *tevPtr, int flags);

/*
 *--------------------------------------------------------------
 *
 * zootcl_state_to_string -- given a zookeeper
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
const char *zootcl_state_to_string (int state)
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

	return "unknown";
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_type_to_string -- given a zookeeper type
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
const char *zootcl_type_to_string (int state)
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

	return "unknown";
}


/*
 *--------------------------------------------------------------
 *
 * zootcl_error_to_code_string -- given a zookeeper error
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
const char *zootcl_error_to_code_string (int status)
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
 * zootcl_stat_to_array -- given an interp, array name
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
int zootcl_stat_to_array (Tcl_Interp *interp, char *arrayName, struct Stat *stat)
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
 * zootcl_set_tcl_return_code -- given a zookeeper status
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
zootcl_set_tcl_return_code (Tcl_Interp *interp, int status) {
	if (status == ZOK) {
		return TCL_OK;
	}
	const char *stateString = zootcl_error_to_code_string (status);

	// NB this needs to be spruced up to set errorCode and a
	// better error message and stuff
	Tcl_SetObjResult (interp, Tcl_NewStringObj (stateString, -1));
	Tcl_SetErrorCode(interp, "ZOOKEEPER", stateString, (char *) NULL);
	return TCL_ERROR;
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_data_completion_callback -- data completion callback function
 *
 * we can't call Tcl directly here because this has occurred
 * asynchronously to whatever the interpreter is doing, so
 * we queue an event to the interpreter instead.
 *
 *--------------------------------------------------------------
 */
void
zootcl_data_completion_callback (int rc, const char *value, int valueLen, const struct Stat *stat, const void *context)
{
	zootcl_callbackEvent *evPtr;
	zootcl_callbackContext *ztc = (zootcl_callbackContext *)context;

	evPtr = ckalloc (sizeof (zootcl_callbackEvent));
	evPtr->event.proc = zootcl_EventProc;

	evPtr->callbackType = DATA;
	evPtr->commandObj = ztc->callbackObj;

	evPtr->data.rc = rc;
	evPtr->data.dataObj = Tcl_NewStringObj (value, valueLen);
	evPtr->data.stat = *stat;
    evPtr->zo = ztc->zo;
	ckfree(ztc);

	Tcl_ThreadQueueEvent (evPtr->zo->threadId, (Tcl_Event *)evPtr, TCL_QUEUE_TAIL);
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_watcher -- watcher callback function
 *
 * we can't call Tcl directly here because this has occurred
 * asynchronously to whatever the interpreter is doing, so
 * we queue an event to the interpreter instead.
 *
 *--------------------------------------------------------------
 */
void zootcl_watcher (zhandle_t *zh, int type, int state, const char *path, void* context)
{
	zootcl_callbackEvent *evPtr;

	evPtr = ckalloc (sizeof (zootcl_callbackEvent));
	evPtr->event.proc = zootcl_EventProc;

	evPtr->callbackType = WATCHER;
    evPtr->zo = (zootcl_objectClientData *)zoo_get_context (zh);
	evPtr->commandObj = (Tcl_Obj *)context;

	evPtr->watcher.type = type;
	evPtr->watcher.state = state;
	evPtr->watcher.path = path;

	Tcl_ThreadQueueEvent (evPtr->zo->threadId, (Tcl_Event *)evPtr, TCL_QUEUE_TAIL);

	// printf("**** zootcl_watcher invoked type '%s' state '%s' path '%s' command '%s'; event queued\n", zootcl_type_to_string (type), zootcl_state_to_string (state), path, Tcl_GetString (evPtr->commandObj));
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_init_callback -- callback function for callbacks
 *   made to the callback passed to zookeeper_init
 *
 * again we can't call Tcl directly here because this has occurred
 * asynchronously to whatever the interpreter is doing, so
 * we queue an event to the interpreter instead.
 *
 *--------------------------------------------------------------
 */
void zootcl_init_callback (zhandle_t *zh, int type, int state, const char *path, void* context)
{
	zootcl_callbackEvent *evPtr;

	// this could be zo = context; because context
	// here is the same as zoo_get_context()'s when
	// the callback is from the callback passed to
	// zookeeper_init
    zootcl_objectClientData *zo = (zootcl_objectClientData *)zoo_get_context (zh);

	// if there's no callback function, return
	if (zo->initCallbackObj == NULL) {
		return;
	}

	evPtr = ckalloc (sizeof (zootcl_callbackEvent));
	evPtr->event.proc = zootcl_EventProc;

	evPtr->callbackType = WATCHER;
    evPtr->zo = zo;
	evPtr->commandObj = zo->initCallbackObj;

	evPtr->watcher.type = type;
	evPtr->watcher.state = state;
	evPtr->watcher.path = path;

	Tcl_ThreadQueueEvent (evPtr->zo->threadId, (Tcl_Event *)evPtr, TCL_QUEUE_TAIL);

	// printf("**** zootcl_watcher invoked type '%s' state '%s' path '%s' command '%s'; event queued\n", zootcl_type_to_string (type), zootcl_state_to_string (state), path, Tcl_GetString (evPtr->commandObj));
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_zookeeperObjectDelete -- command deletion callback routine.
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
zootcl_zookeeperObjectDelete (ClientData clientData)
{
    zootcl_objectClientData *zo = (zootcl_objectClientData *)clientData;

    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	zookeeper_close (zo->zh);
    ckfree((char *)clientData);
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_socket_ready --
 *
 *    this routine is called by Tcl when our channel handler for
 *    the zookeeper socket has something to do
 *
 *    it is set up by the async check
 *
 * Results:
 *    invokes zookeeper_process to notify zookeeper that an event
 *    of interest has happened.
 *
 *----------------------------------------------------------------------
 */
void
zootcl_socket_ready (ClientData clientData, int mask)
{
	int events = 0;
	zootcl_objectClientData *zo = (zootcl_objectClientData *)clientData;
    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	if (mask & TCL_READABLE) {
		events |= ZOOKEEPER_READ;
	}

	if (mask & TCL_WRITABLE) {
		events |= ZOOKEEPER_WRITE;
	}

	int status = zookeeper_process (zo->zh, events);
printf("zookeeper_process status %s\n", zootcl_error_to_code_string (status));

	Tcl_DeleteChannelHandler (zo->channel, zootcl_socket_ready, clientData);
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_EventSetupProc --
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
zootcl_EventSetupProc (ClientData clientData, int flags) {
	Tcl_Time time = {0, 500000};

	Tcl_SetMaxBlockTime (&time);
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_EventCheckProc --
 *
 *    This is a function we pass to Tcl_CreateEventSource that is
 *    invoked to see if any events have occurred and to queue them.
 *
 *    We used this opportunity to find out if zookeeper is interested in
 *    anything (i.e. has watches established or something) asynchronous
 *    and if so interacts with Tcl to have Tcl's event loop detect those
 *    events and call back to us to handle them.
 *
 *    It uses zookeeper_interest to find out what zookeeper is interested
 *    in.  (zookeeper_interest tells us the file descriptor of its socket,
 *    flags indicating if it interesting in reading and/or writing, and
 *    a UNIX timeval to tell us a timeout value to be used with the
 *    select/poll system call.)
 *
 *    Fortunately Tcl has Tcl_SetMaxBlockTime to set that time, so we translate
 *    from UNIX to Tcl and do that.  It forces the application to wake up after
 *    a specific amount of time even if no events have occurred.
 *
 *    If we determine that zookeeper has interest in reads and/or writes then
 *    we create a channel handler to invoke zookeeper_socket_ready for
 *    either or both of those conditions occur.
 *
 * Results:
 *    * Makes a Tcl file channel corresponding to zookeeper's socket if it
 *      doesn't already exist.
 *    * Sets the max block time for Tcl's event loop.
 *    * Arranges a callback to zookeeper_socket_ready if zookeeper is
 *      interested in reads or writes.
 *
 *----------------------------------------------------------------------
 */
void
zootcl_EventCheckProc (ClientData clientData, int flags) {
    zootcl_objectClientData *zo = (zootcl_objectClientData *)clientData;
{
	int fd;
	int interest;
	struct timeval tv;

    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	ZOOAPI zhandle_t *zh = zo->zh;

	// find out what zookeeper is interested in
	int status = zookeeper_interest (zh, &fd, &interest, &tv);
	if ((status != ZOK) && (status != ZNOTHING)) {
		return;
	}

	// if fd is -1 there is no connection
	if (fd == -1) return;

// printf("async check fd %d, interest %d, secs %d, usecs %d\n", fd, interest, tv.tv_sec, tv.tv_usec);

	// convert the struct timeval time-until-zookeeper-wants-another-check
	// to a Tcl_Time
	Tcl_Time time = {tv.tv_sec, tv.tv_usec};
	Tcl_SetMaxBlockTime (&time);

	// create Tcl read and write intereest flags based on
	// the zookeeper-based ones
	int readOrWrite = 0;

	if (interest & ZOOKEEPER_READ) {
		readOrWrite |= TCL_READABLE;
	}

	if (interest & ZOOKEEPER_WRITE) {
		readOrWrite |= TCL_WRITABLE;
	}

	// if readOrWrite is still 0 then zookeeper has no pending interest,
	// we're done
	if (readOrWrite == 0) {
		return;
	}

	// if we haven't done it already, construct a Tcl channel from
	// zookeeper's socket
	if (zo->channel == NULL) {
		zo->channel = Tcl_MakeFileChannel (((void *)(intptr_t) fd), (TCL_READABLE|TCL_WRITABLE));
	}

	Tcl_CreateChannelHandler (zo->channel, readOrWrite, zootcl_socket_ready, (ClientData)zo);
}
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_EventProc --
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
zootcl_EventProc (Tcl_Event *tevPtr, int flags) {
	zootcl_callbackEvent *evPtr = (zootcl_callbackEvent *)tevPtr;
	zootcl_objectClientData *zo = evPtr->zo;
	Tcl_Interp *interp = zo->interp;
	int tclReturnCode;

	int callbackListObjc;
	Tcl_Obj **callbackListObjv;

	int evalObjc;
	Tcl_Obj **evalObjv;

	// printf("zootcl_EventProc invoked\n");

	// crack the command object.  it may be a list of multiple elements
	// and we want that to work, like it could be an object and a method or
	// something.
	if (Tcl_ListObjGetElements (interp, evPtr->commandObj, &callbackListObjc, &callbackListObjv) == TCL_ERROR) {
		Tcl_BackgroundError (interp);
		return 1;
	}

	// construct callback argument as a list
	Tcl_Obj *listObjv[20];
	int element = 0;

	listObjv[element++] = Tcl_NewStringObj ("zk", -1);
	Tcl_Obj *commandObj = Tcl_NewObj();
	Tcl_GetCommandFullName (interp, zo->cmdToken, commandObj);
	listObjv[element++] = commandObj;


	switch(evPtr->callbackType) {
		case WATCHER:
			listObjv[element++] = Tcl_NewStringObj ("path", -1);
			listObjv[element++] = Tcl_NewStringObj (evPtr->watcher.path, -1);

			listObjv[element++] = Tcl_NewStringObj ("type", -1);
			listObjv[element++] = Tcl_NewStringObj (zootcl_type_to_string (evPtr->watcher.type), -1);

			listObjv[element++] = Tcl_NewStringObj ("state", -1);
			listObjv[element++] = Tcl_NewStringObj (zootcl_state_to_string (evPtr->watcher.state), -1);

			break;

		case DATA:
			listObjv[element++] = Tcl_NewStringObj ("status", -1);
			listObjv[element++] = Tcl_NewStringObj (zootcl_error_to_code_string (evPtr->data.rc), -1);
			listObjv[element++] = Tcl_NewStringObj ("data", -1);
			listObjv[element++] = evPtr->data.dataObj;
	}

	Tcl_Obj *listObj = Tcl_NewListObj (element, listObjv);


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
 * zootcl_zookeeperObjectObjCmd --
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
zootcl_zookeeperObjectObjCmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
    zootcl_objectClientData *zo = (zootcl_objectClientData *)clientData;
    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);
	ZOOAPI zhandle_t *zh = zo->zh;
	int optIndex;

    static CONST char *options[] = {
        "get",
        "children",
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
		OPT_CHILDREN,
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
		case OPT_EXISTS:
		case OPT_GET:
		{
			static CONST char *subOptions[] = {
				"-watch",
				"-async",
				"-stat",
				NULL
			};

			enum subOptions {
				SUBOPT_WATCH,
				SUBOPT_ASYNC,
				SUBOPT_STAT
			};

			char *path;
			char buffer[1024*1024];
			int bufferLen = sizeof(buffer);
			watcher_fn wfn = NULL;
			struct Stat *stat = NULL;
			struct Stat statBuf;

			if (objc < 3) {
				Tcl_WrongNumArgs (interp, 2, objv, "path ?-watch code? ?-stat statArray? ?-async command?");
				return TCL_ERROR;
			}

			path = Tcl_GetString (objv[2]);

			int i;
			int suboptIndex = 0;
			Tcl_Obj *watcherCallbackObj = NULL;
			Tcl_Obj *dataCallbackObj = NULL;
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
						watcherCallbackObj = objv[++i];
						Tcl_IncrRefCount (watcherCallbackObj);
						break;
					}

					case SUBOPT_ASYNC:
					{
						if (i + 1 >= objc) {
							Tcl_WrongNumArgs (interp, 2, objv, "-async code");
							return TCL_ERROR;
						}
						dataCallbackObj = objv[++i];
						Tcl_IncrRefCount (dataCallbackObj);
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

			if (watcherCallbackObj != NULL) {
				wfn = zootcl_watcher;
			}

			int status;

			if ((enum options) optIndex == OPT_GET) {
				if (dataCallbackObj != NULL) {
					zootcl_callbackContext *ztc = (zootcl_callbackContext *)ckalloc (sizeof (zootcl_callbackContext));
					ztc->callbackObj = dataCallbackObj;
					ztc->zo = zo;

					status = zoo_awget (zh, path, wfn, (void *)watcherCallbackObj, zootcl_data_completion_callback, ztc);
				} else {
					status = zoo_wget (zh, path, wfn, (void *)watcherCallbackObj, buffer, &bufferLen, stat);
					if (status == ZOK) {
						Tcl_SetObjResult (interp, Tcl_NewStringObj (buffer, bufferLen));
					}
				}
			} else {
				status = zoo_wexists (zh, path, wfn, (void *)watcherCallbackObj, stat);
				if (status == ZOK || status == ZNONODE) {
					Tcl_SetObjResult (interp, Tcl_NewBooleanObj (status == ZOK));
					status = ZOK; // ZNONODE would be an error below but it's ok here
				}
			}

			if (stat != NULL && zootcl_stat_to_array (interp, statArray, stat) == TCL_ERROR) {
				return TCL_ERROR;
			}
			return zootcl_set_tcl_return_code (interp, status);
		}

		case OPT_CHILDREN:
		{
			char *path;
			struct String_vector strings;
			int i;

			if (objc != 3) {
				Tcl_WrongNumArgs (interp, 2, objv, "path");
				return TCL_ERROR;
			}

			path = Tcl_GetString (objv[2]);
			int status = zoo_wget_children (zh, path, NULL, NULL, &strings);
			if (status != ZOK) {
				return zootcl_set_tcl_return_code (interp, status);
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
			return zootcl_set_tcl_return_code (interp, status);
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
			return zootcl_set_tcl_return_code (interp, status);
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
			return zootcl_set_tcl_return_code (interp, status);
		}

		case OPT_STATE:
		{
			if (objc != 2) {
				Tcl_WrongNumArgs (interp, 2, objv, "");
				return TCL_ERROR;
			}

			int state = zoo_state (zh);
			const char *stateString = zootcl_state_to_string (state);
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
 * zootcl_zookeeperObjCmd --
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
zootcl_zookeeperObjCmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
    int                 optIndex;
	zootcl_objectClientData *zo = NULL;

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
			//
			// allocate one of our kafka client data objects for Tcl and configure it
			zo = (zootcl_objectClientData *)ckalloc (sizeof (zootcl_objectClientData));
			zo->zookeeper_object_magic = ZOOKEEPER_OBJECT_MAGIC;
			zo->interp = interp;
			zo->threadId = Tcl_GetCurrentThread ();
			zo->channel = NULL;
			zo->initCallbackObj = NULL;

			zhandle_t *zh = zookeeper_init (hosts, zootcl_init_callback, timeout, NULL, zo, 0);
			// zhandle_t *zh = zookeeper_init (hosts, NULL, timeout, NULL, zo, 0);

			if (zh == NULL) {
				Tcl_SetObjResult (interp, Tcl_NewStringObj (Tcl_PosixError (interp), -1));
				return TCL_ERROR;
			}

			zo->zh = zh;

			zoo_set_context (zo->zh, (void *)zo);

			Tcl_CreateEventSource (zootcl_EventSetupProc, zootcl_EventCheckProc, (ClientData) zo);

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
			zo->cmdToken = Tcl_CreateObjCommand (interp, cmdName, zootcl_zookeeperObjectObjCmd, zo, zootcl_zookeeperObjectDelete);
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




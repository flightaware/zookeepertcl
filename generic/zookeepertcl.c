/* -*- mode: c; tab-width: 4; indent-tabs-mode: t -*- */

/*
 * zootcl - Tcl interface to Apache Zookeeper
 *
 * Copyright (C) 2016 - 2018 FlightAware LLC
 *
 * freely redistributable under the Berkeley license
 */

#include "zookeepertcl.h"
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

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
 * 
 */
/*
 *--------------------------------------------------------------
 *
 * get_in_addr - given a sockaddr struct pointer, return whether
 *  we need a sockaddr_in or a sockaddr_in6
 *
 * http://beej.us/guide/bgnet/examples/client.c
 *
 * Results:
 *      returns pointer to correct sockaddr_in depending on 
 *      whether sa is IPv4 or IPv6
 *
 * Side effects:
 *		none
 *
 *--------------------------------------------------------------
 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
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
	const char *messageString = zerror (status);

	// NB this needs to be spruced up to set errorCode and a
	// better error message and stuff
	Tcl_SetObjResult (interp, Tcl_NewStringObj (messageString, -1));
	Tcl_SetErrorCode(interp, "ZOOKEEPER", stateString, messageString, (char *) NULL);
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

	evPtr->callbackType = DATA_CALLBACK;
	evPtr->commandObj = ztc->callbackObj;

	evPtr->data.rc = rc;

	// if value is NULL then there is no value associated with this znode
	// we set to NULL and the other end (the event handler) will discriminate
	if (value == NULL) {
		evPtr->data.dataObj = NULL;
	} else {
		evPtr->data.dataObj = Tcl_NewStringObj (value, valueLen);
	}

	// structure copy status structure only if it exists
	if (stat != NULL) {
		evPtr->data.stat = *stat;
	}

    evPtr->zo = ztc->zo;
	ckfree(ztc);

	Tcl_ThreadQueueEvent (evPtr->zo->threadId, (Tcl_Event *)evPtr, TCL_QUEUE_TAIL);
	Tcl_ThreadAlert (evPtr->zo->threadId);
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_string_completion_callback -- string completion callback function
 *
 * we can't call Tcl directly here because this has occurred
 * asynchronously to whatever the interpreter is doing, so
 * we queue an event to the interpreter instead.
 *
 *--------------------------------------------------------------
 */
void
zootcl_string_completion_callback (int rc, const char *value, const void *context)
{
	zootcl_callbackEvent *evPtr;
	zootcl_callbackContext *ztc = (zootcl_callbackContext *)context;

	evPtr = ckalloc (sizeof (zootcl_callbackEvent));
	evPtr->event.proc = zootcl_EventProc;

	evPtr->callbackType = STRING_CALLBACK;
	evPtr->commandObj = ztc->callbackObj;
	evPtr->data.rc = rc;

	// if value is NULL then there is no value associated with this znode
	// we set to NULL and the other end (the event handler) will discriminate
	if (value == NULL) {
		evPtr->data.dataObj = NULL;
	} else {
		evPtr->data.dataObj = Tcl_NewStringObj (value, -1);
	}

    evPtr->zo = ztc->zo;
	ckfree(ztc);

	Tcl_ThreadQueueEvent (evPtr->zo->threadId, (Tcl_Event *)evPtr, TCL_QUEUE_TAIL);
	Tcl_ThreadAlert (evPtr->zo->threadId);
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_strings_completion_callback -- multi-string completion callback function
 *
 *--------------------------------------------------------------
 */
void
zootcl_strings_completion_callback (int rc, const struct String_vector *strings, const void *context)
{
	zootcl_callbackEvent *evPtr;
	zootcl_callbackContext *ztc = (zootcl_callbackContext *)context;

	evPtr = ckalloc (sizeof (zootcl_callbackEvent));
	evPtr->event.proc = zootcl_EventProc;

	evPtr->callbackType = STRING_CALLBACK;
	evPtr->commandObj = ztc->callbackObj;
	evPtr->data.rc = rc;
    	evPtr->zo = ztc->zo;
	ckfree(ztc);

	// marshall the zookeeper strings into Tcl string objects
	// and make a Tcl list object of them
        int count = strings ? strings->count : 0;
	Tcl_Obj **listObjv = (Tcl_Obj **)ckalloc (sizeof(Tcl_Obj *) * count);

	int i;
	for (i = 0; i < count; i++) {
		listObjv[i] = Tcl_NewStringObj (strings->data[i], -1);
	}

	Tcl_Obj *listObj = Tcl_NewListObj (count, listObjv);
	ckfree (listObjv);

	evPtr->data.dataObj = listObj;

	Tcl_ThreadQueueEvent (evPtr->zo->threadId, (Tcl_Event *)evPtr, TCL_QUEUE_TAIL);
	Tcl_ThreadAlert (evPtr->zo->threadId);
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_void_completion_callback -- void completion callback function
 *
 *--------------------------------------------------------------
 */
void
zootcl_void_completion_callback (int rc, const void *context)
{
	zootcl_callbackEvent *evPtr;
	zootcl_callbackContext *ztc = (zootcl_callbackContext *)context;

	evPtr = ckalloc (sizeof (zootcl_callbackEvent));
	evPtr->event.proc = zootcl_EventProc;

	evPtr->callbackType = VOID_CALLBACK;
	evPtr->commandObj = ztc->callbackObj;
	evPtr->data.dataObj = NULL;
	evPtr->data.rc = rc;
    evPtr->zo = ztc->zo;
	ckfree(ztc);

	Tcl_ThreadQueueEvent (evPtr->zo->threadId, (Tcl_Event *)evPtr, TCL_QUEUE_TAIL);
	Tcl_ThreadAlert (evPtr->zo->threadId);
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_stat_completion_callback -- string completion callback function
 *
 *--------------------------------------------------------------
 */
void
zootcl_stat_completion_callback (int rc, const struct Stat *stat, const void *context)
{
	zootcl_callbackEvent *evPtr;
	zootcl_callbackContext *ztc = (zootcl_callbackContext *)context;

	evPtr = ckalloc (sizeof (zootcl_callbackEvent));
	evPtr->event.proc = zootcl_EventProc;

	evPtr->callbackType = STAT_CALLBACK;
	evPtr->commandObj = ztc->callbackObj;

	evPtr->data.rc = rc;
	evPtr->data.dataObj = NULL;
    
    if (stat != NULL) {
	    evPtr->data.stat = *stat;
    }

    evPtr->zo = ztc->zo;
	ckfree(ztc);

	Tcl_ThreadQueueEvent (evPtr->zo->threadId, (Tcl_Event *)evPtr, TCL_QUEUE_TAIL);
	Tcl_ThreadAlert (evPtr->zo->threadId);
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_queue_null_event -- queue a "no op" event to wake
 * up the event loop, but the event doesn't actually do anything.
 *
 * this helps our pseudo-synchronous functions to work.
 *
 *--------------------------------------------------------------
 */
void
zootcl_queue_null_event (zootcl_syncCallbackContext *zsc)
{
	zootcl_callbackEvent *evPtr;

	evPtr = ckalloc (sizeof (zootcl_callbackEvent));
	evPtr->event.proc = zootcl_EventProc;
	evPtr->callbackType = NULL_CALLBACK;
	Tcl_ThreadQueueEvent (zsc->zo->threadId, (Tcl_Event *)evPtr, TCL_QUEUE_TAIL);
	Tcl_ThreadAlert (zsc->zo->threadId);
}


/*
 *--------------------------------------------------------------
 *
 * zootcl_sync_stat_completion_callback -- string completion callback function
 *
 *--------------------------------------------------------------
 */
void
zootcl_sync_stat_completion_callback (int rc, const struct Stat *stat, const void *context)
{
	zootcl_syncCallbackContext *zsc = (zootcl_syncCallbackContext *)context;
	zsc->rc = rc;
	if (stat != NULL) {
		zsc->stat = *stat;
	}
	zsc->syncDone = 1;
	zootcl_queue_null_event (zsc);
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_sync_data_completion_callback -- data completion callback function
 *
 *--------------------------------------------------------------
 */
void
zootcl_sync_data_completion_callback (int rc, const char *value, int valueLen, const struct Stat *stat, const void *context)
{
	zootcl_syncCallbackContext *zsc = (zootcl_syncCallbackContext *)context;
	zsc->rc = rc;

	// if value is NULL then there is no value associated with this znode
	// we set to NULL and the other end (the event handler) will discriminate
	if (value == NULL) {
		zsc->dataObj = NULL;
	} else {
		zsc->dataObj = Tcl_NewStringObj (value, valueLen);
	}

	// structure copy status structure only if it exists
	if (stat != NULL) {
		zsc->stat = *stat;
	}
	zsc->syncDone = 1;
	zootcl_queue_null_event (zsc);
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_sync_string_completion_callback -- string completion callback function
 *
 *--------------------------------------------------------------
 */
void
zootcl_sync_string_completion_callback (int rc, const char *value, const void *context)
{
	zootcl_syncCallbackContext *zsc = (zootcl_syncCallbackContext *)context;
	zsc->rc = rc;

	// if value is NULL then there is no value associated with this znode
	// we set to NULL and the other end (the event handler) will discriminate
	if (value == NULL) {
		zsc->dataObj = NULL;
	} else {
		zsc->dataObj = Tcl_NewStringObj (value, -1);
	}
	zsc->syncDone = 1;
	zootcl_queue_null_event (zsc);
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_sync_strings_completion_callback -- multi-string completion callback function
 *
 *--------------------------------------------------------------
 */
void
zootcl_sync_strings_completion_callback (int rc, const struct String_vector *strings, const void *context)
{
	int i;
	zootcl_syncCallbackContext *zsc = (zootcl_syncCallbackContext *)context;
	zsc->rc = rc;

	// marshall the zookeeper strings into Tcl string objects
	// and make a Tcl list object of them
	int count = strings ? strings->count : 0;
	Tcl_Obj **listObjv = (Tcl_Obj **)ckalloc (sizeof(Tcl_Obj *) * count);

	for (i = 0; i < count; i++) {
		listObjv[i] = Tcl_NewStringObj (strings->data[i], -1);
	}

	Tcl_Obj *listObj = Tcl_NewListObj (count, listObjv);
	ckfree (listObjv);

	zsc->dataObj = listObj;
	zsc->syncDone = 1;
	zootcl_queue_null_event (zsc);
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_sync_void_completion_callback -- void completion callback function
 *
 *--------------------------------------------------------------
 */
void
zootcl_sync_void_completion_callback (int rc, const void *context)
{
	zootcl_syncCallbackContext *zsc = (zootcl_syncCallbackContext *)context;
	zsc->rc = rc;
	zsc->syncDone = 1;
	zootcl_queue_null_event (zsc);
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

	evPtr->callbackType = WATCHER_CALLBACK;
    evPtr->zo = (zootcl_objectClientData *)zoo_get_context (zh);
	evPtr->commandObj = (Tcl_Obj *)context;

	evPtr->watcher.type = type;
	evPtr->watcher.state = state;

	evPtr->watcher.path = ckalloc (strlen (path) + 1);
	strcpy (evPtr->watcher.path, path);

	Tcl_ThreadQueueEvent (evPtr->zo->threadId, (Tcl_Event *)evPtr, TCL_QUEUE_TAIL);
	Tcl_ThreadAlert (evPtr->zo->threadId);

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

    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	// if there's no callback function, return
	if (zo->initCallbackObj == NULL) {
		return;
	}

	evPtr = ckalloc (sizeof (zootcl_callbackEvent));
	evPtr->event.proc = zootcl_EventProc;

	evPtr->callbackType = INTERNAL_INIT_CALLBACK;
    evPtr->zo = zo;
	evPtr->commandObj = zo->initCallbackObj;

	evPtr->watcher.type = type;
	evPtr->watcher.state = state;

	evPtr->watcher.path = ckalloc (strlen (path) + 1);
	strcpy (evPtr->watcher.path, path);

	Tcl_ThreadQueueEvent (evPtr->zo->threadId, (Tcl_Event *)evPtr, TCL_QUEUE_TAIL);
	Tcl_ThreadAlert (evPtr->zo->threadId);

	// printf("**** zootcl_watcher invoked type '%s' state '%s' path '%s' command '%s'; event queued\n", zootcl_type_to_string (type), zootcl_state_to_string (state), path, Tcl_GetString (evPtr->commandObj));
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

	if (mask & TCL_EXCEPTION) {
		fprintf (stderr, "SOCKET EXCEPTION\n");
	}

	if (mask & TCL_READABLE) {
		events |= ZOOKEEPER_READ;
	}

	if (mask & TCL_WRITABLE) {
		events |= ZOOKEEPER_WRITE;
	}

	int status = zookeeper_process (zo->zh, events);
	if ((status != ZOK) && (status != ZNOTHING)) {
		if (status == ZCONNECTIONLOSS) {
			// zootcl_init_callback (zo->zh, ZOO_SESSION_EVENT, 0, NULL, NULL);
		}
		fprintf(stderr, "zookeeper_process abnormal status %s, readable %d, writable %d\n", zootcl_error_to_code_string (status), events & ZOOKEEPER_READ ? 1 : 0, events & ZOOKEEPER_WRITE ? 1:0);
	}
// fprintf(stderr,"zookeeper_process status %s, readable %d, writable %d\n", zootcl_error_to_code_string (status), events & ZOOKEEPER_READ ? 1 : 0, events & ZOOKEEPER_WRITE ? 1:0);
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_EventCommonProc --
 *    This routine is a required argument to Tcl_CreateEventSource
 *
 *    This is a function we pass to Tcl_CreateEventSource that is
 *    invoked to see if any events have occurred and to queue them.
 *
 *    We get our events from a channel handler so this function is empty.
 *
 * Results:
 *
 *----------------------------------------------------------------------
 */
void
zootcl_EventCommonProc (ClientData clientData, int flags, int doTime) {
#ifndef TCL_THREADS
    zootcl_objectClientData *zo = (zootcl_objectClientData *)clientData;
	int fd;
	int interest;
	struct timeval tv;

    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	ZOOAPI zhandle_t *zh = zo->zh;

	// find out what zookeeper is interested in
	int status = zookeeper_interest (zh, &fd, &interest, &tv);
// fprintf(stderr, "zootcl_EventCommonProc: status %s, fd %d, interest read %d, write %d, secs %d, usecs %d\n", zootcl_error_to_code_string (status), fd, interest & ZOOKEEPER_READ ? 1 : 0, interest & ZOOKEEPER_WRITE ? 1 : 0, tv.tv_sec, tv.tv_usec);

	if ((zo->currentFD != -1) && (fd != zo->currentFD)) {
// fprintf(stderr,"fd changed from %d to %d!\n", zo->currentFD, fd);
		Tcl_DeleteChannelHandler (zo->channel, zootcl_socket_ready, (ClientData)zo);
		Tcl_DetachChannel (zo->interp, zo->channel);
		zo->channel = NULL;
		zo->currentFD = -1;
	}

	if ((status != ZOK) && (status != ZNOTHING)) {
// fprintf(stderr, "zootcl_EventCommonProc: status %s, fd %d, interest read %d, write %d, secs %d, usecs %d\n", zootcl_error_to_code_string (status), fd, interest & ZOOKEEPER_READ ? 1 : 0, interest & ZOOKEEPER_WRITE ? 1 : 0, tv.tv_sec, tv.tv_usec);
		return;
	}

	if (doTime && tv.tv_sec > 1) {
		/*
		tv.tv_usec -= 500000;
		if (tv.tv_usec < 0) {
			tv.tv_usec += 1000000;
			tv.tv_sec--;
		}
		*/

		// convert the struct timeval time-until-zookeeper-wants-another-check
		// to a Tcl_Time
		Tcl_Time time = {tv.tv_sec, tv.tv_usec};
		// Tcl_Time time = {1, 0};
		Tcl_SetMaxBlockTime (&time);
	}

	// if fd is -1 there is no connection
	if (fd == -1) return;

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
	if (readOrWrite == 0 && zo->channel != NULL) {
		Tcl_DeleteChannelHandler (zo->channel, zootcl_socket_ready, (ClientData)zo);
		return;
	}

	// if we haven't done it already, construct a Tcl channel from
	// zookeeper's socket
	if (zo->channel == NULL) {
		zo->channel = Tcl_MakeFileChannel (((void *)(intptr_t) fd), (TCL_READABLE|TCL_WRITABLE));
		zo->currentFD = fd;
		// assert (Tcl_SetChannelOption (NULL, zo->channel, "-blocking", "0") == TCL_OK);
	}
	Tcl_CreateChannelHandler (zo->channel, (readOrWrite | TCL_EXCEPTION), zootcl_socket_ready, (ClientData)zo);
#endif
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_EventCheckProc --
 *    This routine is a required argument to Tcl_CreateEventSource
 *
 *    This is a function we pass to Tcl_CreateEventSource that is
 *    invoked to see if any events have occurred and to queue them.
 *
 *    We get our events from a channel handler so this function is empty.
 *
 * Results:
 *
 *----------------------------------------------------------------------
 */
void
zootcl_EventCheckProc (ClientData clientData, int flags) {
	zootcl_EventCommonProc (clientData, flags, 0);
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_EventSetupProc --
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
zootcl_EventSetupProc (ClientData clientData, int flags) {
	zootcl_EventCommonProc (clientData, flags, 1);
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
	if (evPtr->callbackType == NULL_CALLBACK) {
		return 1;
	}

	zootcl_objectClientData *zo = evPtr->zo;
	Tcl_Interp *interp = zo->interp;
	int tclReturnCode;

	int callbackListObjc;
	Tcl_Obj **callbackListObjv;

	int evalObjc;
	Tcl_Obj **evalObjv;

    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	// fprintf(stderr, "zootcl_EventProc invoked\n");

	// crack the command object.  it may be a list of multiple elements
	// and we want that to work, like it could be an object and a method or
	// something.
	if (Tcl_ListObjGetElements (interp, evPtr->commandObj, &callbackListObjc, &callbackListObjv) == TCL_ERROR) {
		Tcl_BackgroundError (interp);
		return 1;
	}

	// construct callback argument as a list
	Tcl_Obj *listObjv[40];
	int element = 0;

	listObjv[element++] = Tcl_NewStringObj ("zk", -1);
	Tcl_Obj *commandObj = Tcl_NewObj();
	Tcl_GetCommandFullName (interp, zo->cmdToken, commandObj);
	listObjv[element++] = commandObj;


	switch(evPtr->callbackType) {
		case NULL_CALLBACK:
			// should never reach here
			assert(0 == 1);

		case INTERNAL_INIT_CALLBACK:
		case WATCHER_CALLBACK:
			if (evPtr->watcher.path != NULL) {
					if (*evPtr->watcher.path != '\0') {
						listObjv[element++] = Tcl_NewStringObj ("path", -1);
						listObjv[element++] = Tcl_NewStringObj (evPtr->watcher.path, -1);
					}
					ckfree (evPtr->watcher.path);
					evPtr->watcher.path = NULL;
			}

			listObjv[element++] = Tcl_NewStringObj ("type", -1);
			listObjv[element++] = Tcl_NewStringObj (zootcl_type_to_string (evPtr->watcher.type), -1);

			listObjv[element++] = Tcl_NewStringObj ("state", -1);
			listObjv[element++] = Tcl_NewStringObj (zootcl_state_to_string (evPtr->watcher.state), -1);

			break;

		case VOID_CALLBACK:
		case DATA_CALLBACK:
		case STRING_CALLBACK:
			listObjv[element++] = Tcl_NewStringObj ("status", -1);
			listObjv[element++] = Tcl_NewStringObj (zootcl_error_to_code_string (evPtr->data.rc), -1);

			if (evPtr->data.dataObj != NULL) {
				listObjv[element++] = Tcl_NewStringObj ("data", -1);
				listObjv[element++] = evPtr->data.dataObj;

				if (evPtr->callbackType == DATA_CALLBACK) {
					listObjv[element++] = Tcl_NewStringObj ("version", -1);
					listObjv[element++] = Tcl_NewIntObj (evPtr->data.stat.version);
				}
			}
			break;

		case STAT_CALLBACK:
			listObjv[element++] = Tcl_NewStringObj ("status", -1);
			listObjv[element++] = Tcl_NewStringObj (zootcl_error_to_code_string (evPtr->data.rc), -1);

			listObjv[element++] = Tcl_NewStringObj ("czxid", -1);
			listObjv[element++] = Tcl_NewLongObj (evPtr->data.stat.czxid);

			listObjv[element++] = Tcl_NewStringObj ("mzxid", -1);
			listObjv[element++] = Tcl_NewLongObj (evPtr->data.stat.mzxid);

			listObjv[element++] = Tcl_NewStringObj ("ctime", -1);
			listObjv[element++] = Tcl_NewLongObj (evPtr->data.stat.ctime);

			listObjv[element++] = Tcl_NewStringObj ("mtime", -1);
			listObjv[element++] = Tcl_NewLongObj (evPtr->data.stat.mtime);

			listObjv[element++] = Tcl_NewStringObj ("version", -1);
			listObjv[element++] = Tcl_NewIntObj (evPtr->data.stat.version);

			listObjv[element++] = Tcl_NewStringObj ("cversion", -1);
			listObjv[element++] = Tcl_NewIntObj (evPtr->data.stat.cversion);

			listObjv[element++] = Tcl_NewStringObj ("aversion", -1);
			listObjv[element++] = Tcl_NewIntObj (evPtr->data.stat.aversion);

			listObjv[element++] = Tcl_NewStringObj ("ephemeralOwner", -1);
			listObjv[element++] = Tcl_NewLongObj (evPtr->data.stat.ephemeralOwner);

			listObjv[element++] = Tcl_NewStringObj ("dataLength", -1);
			listObjv[element++] = Tcl_NewIntObj (evPtr->data.stat.dataLength);

			listObjv[element++] = Tcl_NewStringObj ("numChildren", -1);
			listObjv[element++] = Tcl_NewIntObj (evPtr->data.stat.numChildren);

			listObjv[element++] = Tcl_NewStringObj ("pzxid", -1);
			listObjv[element++] = Tcl_NewLongObj (evPtr->data.stat.pzxid);
			break;
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

	Tcl_DeleteExitHandler (zootcl_zookeeperObjectDelete, clientData);
	Tcl_DeleteThreadExitHandler (zootcl_zookeeperObjectDelete, clientData);
	if (zo->channel != NULL) {
		Tcl_DeleteChannelHandler (zo->channel, zootcl_socket_ready, (ClientData)zo);
		Tcl_DetachChannel (zo->interp, zo->channel);
	}
	Tcl_DeleteEventSource (zootcl_EventSetupProc, zootcl_EventCheckProc, (ClientData) zo);

	// we are freeing memory in a sec, clear the magic number
	// so attempt to reuse a freed object will be an assertion
	// failure
    zo->zookeeper_object_magic = -1;

	zookeeper_close (zo->zh);
    ckfree((char *)clientData);
}

/*
 *--------------------------------------------------------------
 *
 * zootcl_wait -- routine to make sync-like zookeeper functions
 *   wait while keeping the event loop alive.
 *
 * this code was derived from Tcl_VwaitObjCmd in the Tcl core
 *
 * it processes events until zsc->syncDone is true.
 * zsc->syncDone is set by the callback routine such as
 * zootcl_stat_sync_completion_callback.
 *
 *--------------------------------------------------------------
 */
int
zootcl_wait (zootcl_objectClientData *zo, zootcl_syncCallbackContext *zsc)
{
    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);
	Tcl_Interp *interp = zo->interp;
	int foundEvent = 1;

    while (!zsc->syncDone && foundEvent) {
		foundEvent = Tcl_DoOneEvent(TCL_ALL_EVENTS);

		if (Tcl_Canceled(interp, TCL_LEAVE_ERR_MSG) == TCL_ERROR) {
			break;
		}

		if (Tcl_LimitExceeded(interp)) {
			Tcl_ResetResult(interp);
			Tcl_SetObjResult(interp, Tcl_NewStringObj("limit exceeded", -1));
			break;
		}
    }

	if (!foundEvent) {
		Tcl_ResetResult(interp);
		Tcl_SetObjResult(interp, Tcl_NewStringObj("can't wait for zookeeper event: would wait forever", -1));
		Tcl_SetErrorCode(interp, "TCL", "EVENT", "NO_SOURCES", NULL);
		return TCL_ERROR;
	}

    if (!zsc->syncDone) {
		return TCL_ERROR;
    }

    /*
     * Clear out the interpreter's result, since it may have been set by event
     * handlers.
     */

    Tcl_ResetResult(interp);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_exists_subcommand --
 *
 *      implement the "exists" methods of a zookeeper tcl command
 *      object
 *
 * Results:
 *      A standard Tcl result.
 *
 *
 *----------------------------------------------------------------------
 */
int
zootcl_exists_subcommand(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[], ZOOAPI zhandle_t *zh, zootcl_objectClientData *zo)
{
	static CONST char *subOptions[] = {
		"-watch",
		"-async",
		"-stat",
		"-version",
		NULL
	};

	enum subOptions {
		SUBOPT_WATCH,
		SUBOPT_ASYNC,
		SUBOPT_STAT,
		SUBOPT_VERSION
	};

	const char *path;
	watcher_fn wfn = NULL;

    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	if (objc < 3) {
		Tcl_WrongNumArgs (interp, 2, objv, "path ?-watch code? ?-stat statArray? ?-async callback? ?-version versionVar?");
		return TCL_ERROR;
	}

	path = Tcl_GetString (objv[2]);

	int i;
	int suboptIndex = 0;
	Tcl_Obj *watcherCallbackObj = NULL;
	Tcl_Obj *asyncCallbackObj = NULL;
	char *statArray = NULL;
	Tcl_Obj *versionVarObj = NULL;

	for (i = 3; i < objc; i++) {
		if (Tcl_GetIndexFromObj (interp, objv[i], subOptions, "suboption",
			TCL_EXACT, &suboptIndex) != TCL_OK) {
			return TCL_ERROR;
		}

		switch ((enum subOptions) suboptIndex) {
			case SUBOPT_WATCH:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "path ... -watch code");
					return TCL_ERROR;
				}
				watcherCallbackObj = objv[++i];
				Tcl_IncrRefCount (watcherCallbackObj);
				break;
			}

			case SUBOPT_ASYNC:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "path ... -async code");
					return TCL_ERROR;
				}
				asyncCallbackObj = objv[++i];
				Tcl_IncrRefCount (asyncCallbackObj);
				break;
			}

			case SUBOPT_STAT:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "path ... -stat statArray");
					return TCL_ERROR;
				}
				statArray = Tcl_GetString (objv[++i]);
				break;
			}

			case SUBOPT_VERSION:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "path ... -version versionVar");
					return TCL_ERROR;
				}
				versionVarObj = objv[++i];
				break;
			}
		}
	}

	if (asyncCallbackObj != NULL) {
		if (statArray != NULL) {
			Tcl_SetObjResult (interp, Tcl_NewStringObj ("-stat and -async options are mutually exclusive", -1));
			return TCL_ERROR;
		}

		if (versionVarObj != NULL) {
			Tcl_SetObjResult (interp, Tcl_NewStringObj ("-version and -async options are mutually exclusive", -1));
			return TCL_ERROR;
		}
	}

	if (watcherCallbackObj != NULL) {
		wfn = zootcl_watcher;
	}

	int status;

	if (asyncCallbackObj == NULL) {
		struct Stat *stat = (struct Stat *)ckalloc (sizeof (struct Stat));
		status = zoo_wexists(zh, path, wfn, (void *)watcherCallbackObj, stat);	

		// if there's no node hand that according to our rule.
		// unset the version var since we don't have one and we
		// don't want to confuse the caller by letting through some
		// old thing or require them to unset the var before calling
		// us.  that ain't happenin'.
		if (status == ZNONODE) {
			if (versionVarObj != NULL) {
				Tcl_UnsetVar (interp, Tcl_GetString (versionVarObj), 0);
			}
			Tcl_SetObjResult (interp, Tcl_NewBooleanObj (0));
			ckfree (stat);
			return TCL_OK;
		}

		// handle errors from the call like bad arguments and stuff
		if (status != ZOK) {
			ckfree (stat);
			return zootcl_set_tcl_return_code (interp, status);
		}

		// it does exist
		Tcl_SetObjResult (interp, Tcl_NewBooleanObj (1));

		// if the caller wanted a stat array, we're good here,
		// fill that.
		if (statArray != NULL && zootcl_stat_to_array (interp, statArray, stat) == TCL_ERROR) {
			ckfree (stat);
			return TCL_ERROR;
		}

		// we want version so commonly and not much else in the stat array
		// so we have a -version option to make it easy
		if (versionVarObj != NULL) {
			if (Tcl_SetVar2Ex (interp, Tcl_GetString (versionVarObj), NULL, Tcl_NewIntObj (stat->version), TCL_LEAVE_ERR_MSG) == NULL) {
				ckfree (stat);
				return TCL_ERROR;
			}
		}
		ckfree (stat);
	} else {
		// do the asynchronous version of znode existence check
		zootcl_callbackContext *ztc = (zootcl_callbackContext *)ckalloc (sizeof (zootcl_callbackContext));
		ztc->callbackObj = asyncCallbackObj;
		ztc->zo = zo;

		status = zoo_awexists (zh, path, wfn, (void *)watcherCallbackObj, zootcl_stat_completion_callback, ztc);
	}

	return zootcl_set_tcl_return_code (interp, status);
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_get_subcommand --
 *
 *      implement the "get" methods of a zookeeper tcl command
 *      object
 *
 * Results:
 *      A standard Tcl result.
 *
 * NB: bears way too much in common with zootcl_exists_subcommand; needs factoring
 *
 *----------------------------------------------------------------------
 */
int
zootcl_get_subcommand(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[], ZOOAPI zhandle_t *zh, zootcl_objectClientData *zo)
{
	static CONST char *subOptions[] = {
		"-watch",
		"-async",
		"-stat",
		"-data",
		"-version",
		NULL
	};

	enum subOptions {
		SUBOPT_WATCH,
		SUBOPT_ASYNC,
		SUBOPT_STAT,
		SUBOPT_DATA,
		SUBOPT_VERSION
	};

	const char *path;
	watcher_fn wfn = NULL;

    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	if (objc < 3) {
		Tcl_WrongNumArgs (interp, 2, objv, "path ?-watch code? ?-stat statArray? ?-async callback? ?-data dataVar? ?-version versionVar?");
		return TCL_ERROR;
	}

	path = Tcl_GetString (objv[2]);

	int i;
	int suboptIndex = 0;
	Tcl_Obj *watcherCallbackObj = NULL;
	Tcl_Obj *asyncCallbackObj = NULL;
	char *statArray = NULL;
	Tcl_Obj *dataVarObj = NULL;
	Tcl_Obj *versionVarObj = NULL;

	for (i = 3; i < objc; i++) {
		if (Tcl_GetIndexFromObj (interp, objv[i], subOptions, "suboption",
			TCL_EXACT, &suboptIndex) != TCL_OK) {
			return TCL_ERROR;
		}

		switch ((enum subOptions) suboptIndex) {
			case SUBOPT_WATCH:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "path ... -watch code");
					return TCL_ERROR;
				}
				watcherCallbackObj = objv[++i];
				Tcl_IncrRefCount (watcherCallbackObj);
				break;
			}

			case SUBOPT_ASYNC:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "path ... -async code");
					return TCL_ERROR;
				}
				asyncCallbackObj = objv[++i];
				Tcl_IncrRefCount (asyncCallbackObj);
				break;
			}

			case SUBOPT_STAT:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "path ... -stat statArray");
					return TCL_ERROR;
				}
				statArray = Tcl_GetString (objv[++i]);
				break;
			}

			case SUBOPT_DATA:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "path ... -data dataVar");
					return TCL_ERROR;
				}
				dataVarObj = objv[++i];
				break;
			}

			case SUBOPT_VERSION:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "path ... -version versionVar");
					return TCL_ERROR;
				}
				versionVarObj = objv[++i];
				break;
			}
		}
	}

	if (asyncCallbackObj != NULL) {
		if (statArray != NULL) {
			Tcl_SetObjResult (interp, Tcl_NewStringObj ("-stat and -async options are mutually exclusive", -1));
			return TCL_ERROR;
		}

		if (dataVarObj != NULL) {
			Tcl_SetObjResult (interp, Tcl_NewStringObj ("-data and -async options are mutually exclusive", -1));
			return TCL_ERROR;
		}

		if (versionVarObj != NULL) {
			Tcl_SetObjResult (interp, Tcl_NewStringObj ("-version and -async options are mutually exclusive", -1));
			return TCL_ERROR;
		}
	}

	if (watcherCallbackObj != NULL) {
		wfn = zootcl_watcher;
	}

	int status;

	// if asyncCallbackObj is null, do the synchronous version
	if (asyncCallbackObj == NULL) {
		char buffer[512];
		int bufferLen = 512;
		struct Stat *stat = (struct Stat *)ckalloc (sizeof (struct Stat));

		status = zoo_wget(zh, path, wfn, (void *)watcherCallbackObj, buffer, &bufferLen, stat);	

		// if the node does not exist and -data was specified
		// unset the var: do the same if a -version var was
		// also specified and, finally, return 0
		if (status == ZNONODE && dataVarObj != NULL) {
			Tcl_UnsetVar(interp, Tcl_GetString(dataVarObj), 0);
			if (versionVarObj != NULL) {
				Tcl_UnsetVar (interp, Tcl_GetString (versionVarObj), 0);
			}
			Tcl_SetObjResult (interp, Tcl_NewBooleanObj (0));
			ckfree (stat);
			return TCL_OK;
		}

		if (status != ZOK) {
			ckfree (stat);
			return zootcl_set_tcl_return_code (interp, status);
		}

		if (dataVarObj == NULL) {
			if (bufferLen != -1) {
				Tcl_SetObjResult (interp, Tcl_NewStringObj(buffer, bufferLen));
			}
		} else if (bufferLen == -1) {
			// if the node does exist, but the data in the znode is NULL
			// unset the var name specified by -data
			Tcl_UnsetVar(interp, Tcl_GetString(dataVarObj), 0);

			// Need to set a boolean result to return to the caller
			Tcl_SetObjResult (interp, Tcl_NewBooleanObj (1));
		} else {
			if (Tcl_SetVar2Ex (interp, Tcl_GetString (dataVarObj), NULL, Tcl_NewStringObj(buffer, bufferLen), TCL_LEAVE_ERR_MSG) == NULL) {
				ckfree (stat);
				return TCL_ERROR;
			}
			Tcl_SetObjResult (interp, Tcl_NewBooleanObj (1));
		}

		if (zootcl_stat_to_array (interp, statArray, stat) == TCL_ERROR) {
			ckfree (stat);
			return TCL_ERROR;
		}

		if (versionVarObj != NULL) {
			if (Tcl_SetVar2Ex (interp, Tcl_GetString (versionVarObj), NULL, Tcl_NewIntObj (stat->version), TCL_LEAVE_ERR_MSG) == NULL) {
				ckfree (stat);
				return TCL_ERROR;
			}
		}
		ckfree (stat);
	} else {
		// do the asynchronous version
		zootcl_callbackContext *ztc = (zootcl_callbackContext *)ckalloc (sizeof (zootcl_callbackContext));
		ztc->callbackObj = asyncCallbackObj;
		ztc->zo = zo;

		status = zoo_awget (zh, path, wfn, (void *)watcherCallbackObj, zootcl_data_completion_callback, ztc);
	}

	return zootcl_set_tcl_return_code (interp, status);
}


/*
 *----------------------------------------------------------------------
 *
 * zootcl_children_subcommand --
 *
 *      implement the "children" method of a zookeeper tcl command
 *      object
 *
 * Results:
 *      A standard Tcl result.
 *
 *
 *----------------------------------------------------------------------
 */
int
zootcl_children_subcommand(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[], ZOOAPI zhandle_t *zh, zootcl_objectClientData *zo)
{
	Tcl_Obj *callbackObj = NULL;
	Tcl_Obj *watcherCallbackObj = NULL;

	static CONST char *subOptions[] = {
		"-watch",
		"-async",
		NULL
	};

	enum subOptions {
		SUBOPT_WATCH,
		SUBOPT_ASYNC
	};

	char *path;
	int i;
	int suboptIndex = 0;
	int status;
	watcher_fn wfn = NULL;

    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	if ((objc < 3) || (objc > 7)) {
		Tcl_WrongNumArgs (interp, 2, objv, "path ?-async callback? ?-watch code?");
		return TCL_ERROR;
	}

	path = Tcl_GetString (objv[2]);

	for (i = 3; i < objc; i++) {
		if (Tcl_GetIndexFromObj (interp, objv[i], subOptions, "suboption",
			TCL_EXACT, &suboptIndex) != TCL_OK) {
			return TCL_ERROR;
		}

		switch ((enum subOptions) suboptIndex) {
			case SUBOPT_WATCH:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "path ... -watch code");
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
				callbackObj = objv[++i];
				Tcl_IncrRefCount (callbackObj);
				break;
			}
		}
	}

	if (watcherCallbackObj != NULL) {
		wfn = zootcl_watcher;
	}


	if (callbackObj == NULL) {
		struct String_vector *strings = (struct String_vector *)ckalloc (sizeof (struct String_vector));
		status = zoo_wget_children(zh, path, wfn, (void *)watcherCallbackObj, strings);	

		if (status != ZOK && status != ZNONODE) {
			ckfree (strings);
			return zootcl_set_tcl_return_code (interp, status);
		} else {
			// do not consider a non-existent path to be an error in this case
			status = ZOK;
		}
		
		int count = strings ? strings->count : 0;
		Tcl_Obj **listObjv = (Tcl_Obj **)ckalloc (sizeof(Tcl_Obj *) * count);

		for (i = 0; i < count; i++) {
			listObjv[i] = Tcl_NewStringObj (strings->data[i], -1);
		}

		Tcl_Obj *listObj = Tcl_NewListObj (count, listObjv);
		
		Tcl_SetObjResult (interp, listObj);
		ckfree (strings);
	} else {
		zootcl_callbackContext *ztc = (zootcl_callbackContext *)ckalloc (sizeof (zootcl_callbackContext));
		ztc->callbackObj = callbackObj;
		ztc->zo = zo;
		status = zoo_awget_children (zh, path, wfn, watcherCallbackObj, zootcl_strings_completion_callback, ztc);
	}

	return zootcl_set_tcl_return_code (interp, status);
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_set_subcommand --
 *
 *      implement the "set" method of a zookeeper tcl command
 *      object
 *
 * Results:
 *      A standard Tcl result.
 *
 *
 *----------------------------------------------------------------------
 */
int
zootcl_set_subcommand(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[], ZOOAPI zhandle_t *zh, zootcl_objectClientData *zo)
{
	Tcl_Obj *callbackObj = NULL;

	static CONST char *subOptions[] = {
		"-async",
		NULL
	};

	enum subOptions {
		SUBOPT_ASYNC
	};

	char *path;
	char *buffer;
	int bufferLen = 0;
	int version = 0;

	int i;
	int suboptIndex = 0;

    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	if ((objc < 5) || (objc > 7)) {
		Tcl_WrongNumArgs (interp, 2, objv, "path data version ?-async callback?");
		return TCL_ERROR;
	}

	path = Tcl_GetString (objv[2]);
	buffer = Tcl_GetStringFromObj (objv[3], &bufferLen);

	if (Tcl_GetIntFromObj (interp, objv[4], &version) == TCL_ERROR) {
		return TCL_ERROR;
	}

	for (i = 5; i < objc; i++) {
		if (Tcl_GetIndexFromObj (interp, objv[i], subOptions, "suboption",
			TCL_EXACT, &suboptIndex) != TCL_OK) {
			return TCL_ERROR;
		}

		switch ((enum subOptions) suboptIndex) {
			case SUBOPT_ASYNC:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "-async code");
					return TCL_ERROR;
				}
				callbackObj = objv[++i];
				Tcl_IncrRefCount (callbackObj);
				break;
			}
		}
	}

	int status;

	if (callbackObj == NULL) {
		// synchronous set
		struct Stat *stat = (struct Stat *)ckalloc (sizeof (struct Stat));
		status = zoo_set2(zh, path, buffer, bufferLen, version, stat);

		ckfree (stat);
	} else {
		// asynchronous set
		zootcl_callbackContext *ztc = (zootcl_callbackContext *)ckalloc (sizeof (zootcl_callbackContext));
		ztc->zo = zo;
		ztc->callbackObj = callbackObj;
		status = zoo_aset (zh, path, buffer, bufferLen, version, zootcl_stat_completion_callback, ztc);
	}
	return zootcl_set_tcl_return_code (interp, status);
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_create_subcommand --
 *
 *      implement the "create" method of a zookeeper tcl command
 *      object
 *
 * Results:
 *      A standard Tcl result.
 *
 *
 *----------------------------------------------------------------------
 */
int
zootcl_create_subcommand(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[], ZOOAPI zhandle_t *zh, zootcl_objectClientData *zo)
{
	static CONST char *subOptions[] = {
		"-async",
		"-value",
		"-ephemeral",
		"-sequence",
		NULL
	};

	enum subOptions {
		SUBOPT_ASYNC,
		SUBOPT_VALUE,
		SUBOPT_EPHEMERAL,
		SUBOPT_SEQUENCE
	};

	char *path;
	int valueLen = -1;
	char *value = NULL;
	int flags = 0;

    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	if (objc < 3)  {
		Tcl_WrongNumArgs (interp, 2, objv, "path ?-value value? ?-ephemeral? ?-sequence? ?-async callback?");
		return TCL_ERROR;
	}
	path = Tcl_GetString (objv[2]);
	int i;
	int suboptIndex = 0;
	Tcl_Obj *callbackObj = NULL;

	for (i = 3; i < objc; i++) {
		if (Tcl_GetIndexFromObj (interp, objv[i], subOptions, "suboption",
			TCL_EXACT, &suboptIndex) != TCL_OK) {
			return TCL_ERROR;
		}

		switch ((enum subOptions) suboptIndex) {
			case SUBOPT_ASYNC:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "-async callback");
					return TCL_ERROR;
				}
				callbackObj = objv[++i];
				Tcl_IncrRefCount (callbackObj);
				break;
			}
			case SUBOPT_VALUE:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "-value value");
					return TCL_ERROR;
				}
				value = Tcl_GetStringFromObj (objv[++i], &valueLen);
				break;
			}

			case SUBOPT_EPHEMERAL:
			{
				flags |= ZOO_EPHEMERAL;
				break;
			}

			case SUBOPT_SEQUENCE:
			{
				flags |= ZOO_SEQUENCE;
				break;
			}
		}
	}

	int status;

	if (callbackObj == NULL) {
		zootcl_syncCallbackContext *zsc = (zootcl_syncCallbackContext *)ckalloc (sizeof (zootcl_syncCallbackContext));
		zsc->zo = zo;
		zsc->syncDone = 0;

		status = zoo_acreate (zh, path, value, valueLen, &ZOO_OPEN_ACL_UNSAFE, flags, zootcl_sync_string_completion_callback, zsc);
		if (status != ZOK) {
			ckfree (zsc);
			return zootcl_set_tcl_return_code (interp, status);
		}
		if (zootcl_wait (zo, zsc) == TCL_ERROR) {
			ckfree (zsc);
			return TCL_ERROR;
		}
		status = zsc->rc;
		if (status == ZOK) {
			Tcl_SetObjResult (interp, zsc->dataObj);
		}
		ckfree (zsc);
	} else {
		zootcl_callbackContext *ztc = (zootcl_callbackContext *)ckalloc (sizeof (zootcl_callbackContext));
		ztc->callbackObj = callbackObj;
		ztc->zo = zo;
		status = zoo_acreate (zh, path, value, valueLen, &ZOO_OPEN_ACL_UNSAFE, flags, zootcl_string_completion_callback, ztc);
	}
	return zootcl_set_tcl_return_code (interp, status);
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_delete_subcommand --
 *
 *      implement the "delete" method of a zookeeper tcl command
 *      object
 *
 * Results:
 *      A standard Tcl result.
 *
 *
 *----------------------------------------------------------------------
 */
int
zootcl_delete_subcommand(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[], ZOOAPI zhandle_t *zh, zootcl_objectClientData *zo)
{
	Tcl_Obj *callbackObj = NULL;
	int status;

	static CONST char *subOptions[] = {
		"-async",
		NULL
	};

	enum subOptions {
		SUBOPT_ASYNC
	};

	char *path;
	int version = 0;
	int i;
	int suboptIndex = 0;

    assert (zo->zookeeper_object_magic == ZOOKEEPER_OBJECT_MAGIC);

	if ((objc < 4) || (objc > 6)) {
		Tcl_WrongNumArgs (interp, 2, objv, "path version ?-async callback?");
		return TCL_ERROR;
	}

	path = Tcl_GetString (objv[2]);

	if (Tcl_GetIntFromObj (interp, objv[3], &version) == TCL_ERROR) {
		return TCL_ERROR;
	}

	for (i = 4; i < objc; i++) {
		if (Tcl_GetIndexFromObj (interp, objv[i], subOptions, "suboption",
			TCL_EXACT, &suboptIndex) != TCL_OK) {
			return TCL_ERROR;
		}

		switch ((enum subOptions) suboptIndex) {
			case SUBOPT_ASYNC:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "-async code");
					return TCL_ERROR;
				}
				callbackObj = objv[++i];
				Tcl_IncrRefCount (callbackObj);
				break;
			}
		}
	}

	if (callbackObj == NULL) {
		// synchronous delete
		status = zoo_delete(zh, path, version);
		if (status != ZOK) {
			return zootcl_set_tcl_return_code (interp, status);
		}
	} else {
		zootcl_callbackContext *ztc = (zootcl_callbackContext *)ckalloc (sizeof (zootcl_callbackContext));
		ztc->callbackObj = callbackObj;
		ztc->zo = zo;
		status = zoo_adelete (zh, path, version, zootcl_void_completion_callback, ztc);
	}

	return zootcl_set_tcl_return_code (interp, status);
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_destroy_subcommand --
 *
 *      implement the "destroy" method of a zookeeper tcl command
 *      object
 *
 * Results:
 *      Always returns TCL_OK whether or not anything in this function
 *		succeeds
 *
 *
 *----------------------------------------------------------------------
 */
int
zootcl_destroy_subcommand(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[], ZOOAPI zhandle_t *zh, zootcl_objectClientData *zo)
{
	// Close Zookeeper
	zookeeper_close(zo->zh);
	zo->zh = NULL;

	// Remove the command exit handler and delete the command
	Tcl_CmdInfo *infoPtr = (Tcl_CmdInfo *) ckalloc (sizeof (Tcl_CmdInfo));
	infoPtr->deleteProc = NULL;
	Tcl_SetCommandInfoFromToken(zo->cmdToken, infoPtr);
	ckfree(infoPtr);
	Tcl_DeleteCommandFromToken(interp, zo->cmdToken);

	// Get rid of all event handlers and event sources
	Tcl_DeleteExitHandler(zootcl_zookeeperObjectDelete, (ClientData)zo);
	Tcl_DeleteThreadExitHandler(zootcl_zookeeperObjectDelete, (ClientData)zo);
	if (zo->channel != NULL) {
		Tcl_DeleteChannelHandler(zo->channel, zootcl_socket_ready, (ClientData)zo);
		Tcl_DetachChannel(zo->interp, zo->channel);
	}
	Tcl_DeleteEventSource(zootcl_EventSetupProc, zootcl_EventCheckProc, (ClientData)zo);

	// Free memory and get outta here
	ckfree(zo);
	return TCL_OK;
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
		"server",
        "recv_timeout",
        "is_unrecoverable",
		"close",
		"destroy",
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
		OPT_SERVER,
		OPT_RECV_TIMEOUT,
		OPT_IS_UNRECOVERABLE,
		OPT_CLOSE,
		OPT_DESTROY
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

	// hand off each subcommand to its proper handler
    switch ((enum options) optIndex) {
		case OPT_EXISTS:
			return zootcl_exists_subcommand(interp, objc, objv, zh, zo);

		case OPT_GET:
			return zootcl_get_subcommand(interp, objc, objv, zh, zo);

		case OPT_CHILDREN:
			return zootcl_children_subcommand(interp, objc, objv, zh, zo);

		case OPT_SET:
			return zootcl_set_subcommand(interp, objc, objv, zh, zo);

		case OPT_CREATE:
			return zootcl_create_subcommand(interp, objc, objv, zh, zo);

		case OPT_DELETE:
			return zootcl_delete_subcommand(interp, objc, objv, zh, zo);

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

		case OPT_SERVER:
		{
			struct sockaddr sa;
			socklen_t sa_len = sizeof sa; 
			int res;
			char host[1024];

			if (zookeeper_get_connected_host(zh, &sa, &sa_len)) {
				res = getnameinfo(&sa, sa_len, host, sizeof host, NULL, 0, 0);
				if (res != 0) {
					strcpy(host, gai_strerror(res));
				}
			} else {
				strcpy(host, "");
			}

			Tcl_SetObjResult(interp, Tcl_NewStringObj(host, -1));
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

		case OPT_CLOSE:
		case OPT_DESTROY:
			return zootcl_destroy_subcommand(interp, objc, objv, zh, zo);

	}

    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * zootcl_init_subcommand --
 *
 *      implement "init" subcommand of zookeeper::zookeeper command
 *
 * Results:
 *		Connects to zookeeper.
 *		Creates a new zookeeper object.
 *      A standard Tcl result.
 *
 *
 *----------------------------------------------------------------------
 */
int
zootcl_init_subcommand(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	zootcl_objectClientData *zo = NULL;
	int timeout;
	Tcl_Obj *callbackObj = NULL;

	static CONST char *subOptions[] = {
		"-async",
		NULL
	};

	enum subOptions {
		SUBOPT_ASYNC
	};


	if ((objc < 5) || (objc > 7)) {
		Tcl_WrongNumArgs (interp, 2, objv, "cmdName hosts timeout ?-async callback?");
		return TCL_ERROR;
	}


	if (Tcl_GetIntFromObj (interp, objv[4], &timeout) == TCL_ERROR) {
		return TCL_ERROR;
	}

	char *cmdName = Tcl_GetString (objv[2]);
	char *hosts = Tcl_GetString (objv[3]);
	int suboptIndex = 0;
	int i;

	for (i = 5; i < objc; i++) {
		if (Tcl_GetIndexFromObj (interp, objv[i], subOptions, "suboption",
			TCL_EXACT, &suboptIndex) != TCL_OK) {
			return TCL_ERROR;
		}

		switch ((enum subOptions) suboptIndex) {
			case SUBOPT_ASYNC:
			{
				if (i + 1 >= objc) {
					Tcl_WrongNumArgs (interp, 2, objv, "-async code");
					return TCL_ERROR;
				}
				callbackObj = objv[++i];
				Tcl_IncrRefCount (callbackObj);
				break;
			}
		}
	}
	//
	// allocate one of our zookeeper client data objects for Tcl and configure it
	zo = (zootcl_objectClientData *)ckalloc (sizeof (zootcl_objectClientData));
	zo->zookeeper_object_magic = ZOOKEEPER_OBJECT_MAGIC;
	zo->interp = interp;
	zo->threadId = Tcl_GetCurrentThread ();
	zo->channel = NULL;
	zo->currentFD = -1;
	zo->initCallbackObj = callbackObj;

	zhandle_t *zh = zookeeper_init (hosts, zootcl_init_callback, timeout, NULL, zo, 0);

	if (zh == NULL) {
		Tcl_SetObjResult (interp, Tcl_NewStringObj (Tcl_PosixError (interp), -1));
		return TCL_ERROR;
	}

	zo->zh = zh;
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
	zo->cmdToken = Tcl_CreateObjCommand (interp, cmdName, zootcl_zookeeperObjectObjCmd, zo, zootcl_zookeeperObjectDelete);
	Tcl_CreateExitHandler (zootcl_zookeeperObjectDelete, zo);
	Tcl_CreateThreadExitHandler (zootcl_zookeeperObjectDelete, zo);
	Tcl_SetObjResult (interp, Tcl_NewStringObj (cmdName, -1));
	if (autoGeneratedName == 1) {
		ckfree(cmdName);
	}

	Tcl_CreateEventSource (zootcl_EventSetupProc, zootcl_EventCheckProc, (ClientData) zo);
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

		case OPT_INIT:
			return zootcl_init_subcommand(interp, objc, objv);

		case OPT_DEBUG_LEVEL:
		{
			int zooLogLevel = 0;
			int subOption = 0;
			static CONST char *subOptions[] = {
				"error",
				"warn",
				"info",
				"debug",
				"none",
				NULL
			};

			enum subOptions {
				SUBOPT_ERROR,
				SUBOPT_WARN,
				SUBOPT_INFO,
				SUBOPT_DEBUG,
				SUBOPT_NONE
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

				case SUBOPT_NONE:
					zooLogLevel = (ZooLogLevel) 0;
					break;
			}

			zoo_set_debug_level (zooLogLevel);
			break;
		}

	}

    return TCL_OK;
}

/* vim: set ts=4 sw=4 sts=4 noet : */



/* -*- mode: c; tab-width: 4; indent-tabs-mode: t -*- */

/*
 *
 * Include file for zookeepertcl package
 *
 * Copyright (C) 2016 - 2017 by FlightAware, All Rights Reserved
 *
 * Freely redistributable under the Berkeley copyright, see license.terms
 * for details.
 */

#include <tcl.h>
#include <string.h>
#include <zookeeper/zookeeper.h>
#include <zookeeper/zookeeper_version.h>

#define ZOOKEEPER_OBJECT_MAGIC 7220331

extern int
zootcl_zookeeperObjCmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objvp[]);

// this is the data structure we have to throw around between
// zookeeper and zookeepertcl to be able to find one from the other
typedef struct zootcl_objectClientData
{
    int zookeeper_object_magic;
    Tcl_Interp *interp;
	ZOOAPI zhandle_t *zh;
	Tcl_ThreadId threadId;
	Tcl_Command cmdToken;
	Tcl_Channel channel;
	int currentFD;
	Tcl_Obj *initCallbackObj; // handle callbacks from zookeeper_init callback function
} zootcl_objectClientData;

enum zootcl_CallbackType {WATCHER_CALLBACK, DATA_CALLBACK, STRING_CALLBACK, VOID_CALLBACK, STAT_CALLBACK};

typedef struct zootcl_callbackContext
{
	zootcl_objectClientData *zo;
	Tcl_Obj *callbackObj;
} zootcl_callbackContext;

// this is the data structure that zookeepertcl queues to tcl
// to move an event from zookeeper into tcl
typedef struct zootcl_callbackEvent
{
    Tcl_Event event;
	zootcl_objectClientData *zo;
	Tcl_Obj *commandObj;
	enum zootcl_CallbackType callbackType;
	union {
		struct {
			int type;
			int state;
			const char *path;
		} watcher;
		struct {
			int rc;
			Tcl_Obj *dataObj;
			struct Stat stat;
		} data;
	};
} zootcl_callbackEvent;

/* vim: set ts=4 sw=4 sts=4 noet : */


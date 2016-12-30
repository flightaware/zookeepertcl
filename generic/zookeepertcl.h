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
zookeepertcl_zookeeperObjCmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objvp[]);

typedef struct zookeepertcl_objectClientData
{
    int zookeeper_object_magic;
    Tcl_Interp *interp;
	ZOOAPI zhandle_t *zh;
	Tcl_Command cmdToken;
} zookeepertcl_objectClientData;

typedef struct zookeepertcl_callbackEvent
{
    Tcl_Event event;
	zookeepertcl_objectClientData *zo;
	int zookeeperType;
	int zookeeperState;
	const char *path;
	Tcl_Obj *commandObj;
} zookeepertcl_callbackEvent;


/* vim: set ts=4 sw=4 sts=4 noet : */


//
// Created by akainq on 12.10.17.
//



#ifndef FAST_JSONRPC_HTTP_HANDLER_H
#define FAST_JSONRPC_HTTP_HANDLER_H


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <event.h>
#include <evhttp.h>
#include <json-c/json.h>
#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>
#include <lua5.3/lauxlib.h>
#include <memory.h>
#include <ctype.h>
#include <uuid/uuid.h>

typedef  enum {false,true} bool;

typedef struct t_params{

    json_type type;
    void * value;

} params;

typedef  enum {JSON_PARSE_ERROR,METHOD_NOT_FOUND, ARGUMENTS_NUM_ERROR} SERVICE_ERROR_TYPE;


void request_handler(struct evhttp_request *req, void *arg);

#endif //FAST_JSONRPC_HTTP_HANDLER_H
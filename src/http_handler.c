//
// Created by akainq on 12.10.17.
//
#include "include/jsonrpcd.h"
#include "include/http_handler.h"
#include <json-c/json.h>



static void server_job_function(struct job *job, lua_State * L) {


    struct evhttp_request *req = job->user_data;

    ///Response buffer
    struct evbuffer *returnbuffer = evbuffer_new();

    ///Request buffer
    size_t len2 = evbuffer_get_length(evhttp_request_get_input_buffer(req));
    struct evbuffer *reqbuffer = evhttp_request_get_input_buffer(req);
    char *data = malloc(len2 * sizeof(char));
    evbuffer_copyout(reqbuffer, data, len2);
    data[len2] = '\0'; //Set string terminator

    ///JSONRPC 2.0
    enum json_tokener_error jerror;
    struct json_object *jobj_in =  json_tokener_parse_verbose(data,&jerror);

    const char * error_desc = json_tokener_error_desc(jerror);

    enum json_type type;
    char *   jsonrpc_ver = NULL;
    char *   jsonrpc_method= NULL;
    params *  jsonrpc_params= NULL;
    int       jsonrpc_params_len = 0;
    char *   jsonrpc_id= NULL;
    bool     service_error = false;
    SERVICE_ERROR_TYPE error_type;

    if(jobj_in!=NULL) {
        json_object_object_foreach(jobj_in, key, val) {
            type = json_object_get_type(val);

            switch (type) {
                case json_type_string:

                    if (strcmp(key, "jsonrpc") == 0) {
                        jsonrpc_ver = strdup(json_object_get_string(val));
                    }

                    if (strcmp(key, "method") == 0) {
                        jsonrpc_method = strdup(json_object_get_string(val));
                    }

                    if (strcmp(key, "id") == 0) {
                        jsonrpc_id = strdup(json_object_get_string(val));
                    }
                    break;

                case json_type_int:
                    if (strcmp(key, "id") == 0) {
                        char str[12];
                        sprintf(str, "%d", json_object_get_int(val));
                        jsonrpc_id = strdup(str);
                    }

                    break;

                case json_type_array:
                    if (strcmp(key, "params") == 0) {

                        jsonrpc_params_len = json_object_array_length(val);
                        jsonrpc_params = calloc(jsonrpc_params_len, sizeof(params*));

                        for (int i = 0; i < jsonrpc_params_len; i++) {
                            struct json_object * tmp_ar_ob =  json_object_array_get_idx(val,i);
                            type = json_object_get_type(tmp_ar_ob);
                            jsonrpc_params[i].type = type;

                            switch (type) {
                                case json_type_string:
                                    jsonrpc_params[i].value = strdup(json_object_get_string(tmp_ar_ob));
                                    break;
                                case json_type_int:
                                    jsonrpc_params[i].value  = calloc(1, sizeof(void*));
                                    jsonrpc_params[i].value  = (void*)json_object_get_int(tmp_ar_ob);

                                    break;
                                case json_type_object:

                                    break;

                            }
                        }
                    }

                    break;
            }
        }
    }else {
        service_error = true;
        error_type = JSON_PARSE_ERROR;
    }
    ///JSONRPC END


    ///LUA SCRIPT START
    json_object *jobj = json_object_new_object();
    const char *cstr2;



     if(!service_error) {
         int result = lua_pcall(L, 0, 0, 0);
         char str_method[50] = "jsonrpc_";

         strcat(str_method,jsonrpc_method);

         /// lua_Debug ldb;
         // lua_getinfo(L, ">S", &ldb);
         // lua_getstack(L,0, &ldb);
         // lua_getinfo(L, ">nSl", &ldb);




         lua_getglobal(L,"get_num_args");
         lua_pushlstring(L,str_method, strlen(str_method));
         lua_pcall(L, 1, 1, 0);
         const int arg_num = atoi(lua_tostring(L, -1));
         int idx11 = lua_gettop(L);
         lua_pop(L,1);



                if (arg_num == jsonrpc_params_len) {

                    int status = lua_getglobal(L, str_method);
                    if (status != 0) {

                        for (int k = 0; k < jsonrpc_params_len; k++) {

                            switch (jsonrpc_params[k].type) {

                                case json_type_string:
                                    lua_pushlstring(L, jsonrpc_params[k].value, strlen(jsonrpc_params[k].value));
                                    break;

                                case json_type_int:
                                    lua_pushnumber(L, (int)jsonrpc_params[k].value);
                                    break;
                            }

                        }


                        result = lua_pcall(L, jsonrpc_params_len, 1, 0);
                        if (result) {
                            fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
                            return;
                        }
                        size_t len;
                        //  const char *cstr = lua_tolstring(L, -1, &len);

                        int idx = lua_gettop(L);

                        lua_pushvalue(L, 1);
                        lua_pushnil(L);


                        while (lua_next(L, -2) != 0) {
                            idx = lua_gettop(L);

                            const char *key = lua_tostring(L, -2);
                            const char *value = lua_tostring(L, -1);
                            json_object *jstring = json_object_new_string(value);
                            json_object_object_add(jobj, key, jstring);
                            lua_pop(L, 1);

                        }
                        cstr2 = json_object_to_json_string(jobj);
                        lua_pop(L, 2);
                    } else {
                        service_error = true;
                        error_type = METHOD_NOT_FOUND;
                    }
                } else {
                    service_error = true;
                    error_type = ARGUMENTS_NUM_ERROR;

                }
            }

    ///LUA END

    //struct json_object *jobj  = json_tokener_parse(data);
    //cstr2 = json_object_to_json_string(jobj);

    if(service_error)
    {
        ///Prepare and send response ERROR
        switch (error_type) {

            case JSON_PARSE_ERROR:
                evbuffer_add_printf(returnbuffer, "{\"jsonrpc\": \"2.0\",\"error\":{\"code\": -32700, \"message\": \"Parse error\", \"data\": \"%s\"}, \"id\": null}", error_desc);
                break;

            case METHOD_NOT_FOUND:
                evbuffer_add_printf(returnbuffer, "{\"jsonrpc\": \"2.0\",\"error\":{\"code\": -32601, \"message\": \"Method not found\"}, \"id\":\"%s\" }", jsonrpc_id);
                break;

            case ARGUMENTS_NUM_ERROR:
                evbuffer_add_printf(returnbuffer, "{\"jsonrpc\": \"2.0\",\"error\":{\"code\": -32000, \"message\": \"Wrong number of arguments\"}, \"id\":\"%s\" }", jsonrpc_id);
                break;
        }

    } else {

        ///Prepare and send response
        evbuffer_add_printf(returnbuffer, "{\"jsonrpc\": \"%s\", \"result\":\"%s\", \"id\": \"%s\"}", jsonrpc_ver,
                            cstr2, jsonrpc_id);
    }



    evhttp_send_reply(req, HTTP_OK, "Client", returnbuffer);


    free(data);
    free(jobj);

    if(jsonrpc_method)
        free(jsonrpc_method);
    if(jsonrpc_ver)
        free(jsonrpc_ver);
    if(jsonrpc_id)
        free(jsonrpc_id);
    if(jsonrpc_params)
     free(jsonrpc_params);
    evbuffer_free(returnbuffer);
    //evbuffer_free(reqbuffer);
    free(job);


}



void request_handler(struct evhttp_request *req, void *arg) {

    workqueue_t *workqueue = (workqueue_t *)arg;
    job_t *job;
    if ((job = malloc(sizeof(*job))) == NULL) {
        return;

    }
    job->job_function = server_job_function;
    job->user_data = req;

    workqueue_add_job(workqueue, job);

}
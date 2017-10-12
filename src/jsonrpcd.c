//
// Created by akainq on 12.10.17.
//
#define _GNU_SOURCE
#include <memory.h>
#include "include/http_handler.h"
#include "include/jsonrpcd.h"
#include <libconfig.h>

const char *argp_program_version = "fast-jsonrpcd 0.01a";
const char *argp_program_bug_address = "<aka.inq@gmail.com>";

void lua_init(){

    L = luaL_newstate();
    luaL_openlibs(L);

    char * filename_buff = calloc(256, sizeof(char));

    sprintf(filename_buff,"%s/globals.lua",ROOT_DIR);
    int status = luaL_dofile(L,filename_buff);
    if (status) {
        fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
    }

    sprintf(filename_buff,"%s/main.lua",ROOT_DIR);
    status = luaL_loadfile(L,filename_buff);

    if (status) {
        fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
    }

    free(filename_buff);

}

int main(int argc, char **argv) {


    config_t cfg;
    config_setting_t *setting;

    config_init(&cfg);

    const char * path = getenv("JSONRPCD_PATH");

    const char *  config_path;

    if(path)
    asprintf(&config_path,"%s/%s",path,"jsonrpcd.cfg");
    else
    asprintf(&config_path,"%s","jsonrpcd.cfg");

    if (!config_read_file(&cfg,config_path )) {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return (EXIT_FAILURE);
    }
    const char *hostname;
    if (!config_lookup_string(&cfg, "dev.Network.Host", &hostname)){
        printf("Config error name: %s\n\n", hostname);
    }

    const int *port;
    if (!config_lookup_int(&cfg, "dev.Network.Port", &port)){
        printf("Config error name: %d\n\n", port);
    }


    if (!config_lookup_string(&cfg, "dev.Application.root", &ROOT_DIR)){
        printf("Config error name: %s\n\n", ROOT_DIR);
    }

    const u_short        http_port = (u_short)port;
    const char          *http_addr = hostname;
    struct evhttp *http_server = NULL;


    lua_init();
    event_init();
    http_server = evhttp_start(http_addr, http_port);
    evhttp_set_gencb(http_server, request_handler, NULL);
    fprintf(stderr, "%s\n", logo);
    fprintf(stderr, " \n");
    fprintf(stderr, " \n");
    fprintf(stderr, "Server started on port %d...\n", http_port);
    event_dispatch();

    lua_close(L);   /* Cya, Lua */
    free(config_path);

    return(0);
}


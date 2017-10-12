//
// Created by akainq on 12.10.17.
//

#ifndef FAST_JSONRPC_JSONRPCD_H
#define FAST_JSONRPC_JSONRPCD_H
#include <json-c/json.h>
#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>
#include <lua5.3/lauxlib.h>
#include <argp.h>

char * ROOT_DIR;
lua_State *L;


static char doc[] = "...";
static char logo[] = "    _____   ______     ___   ____  _____      _______     _______     ______ \n"
        "   |_   _|.' ____ \\  .'   `.|_   \\|_   _|    |_   __ \\   |_   __ \\  .' ___  |\n"
        "     | |  | (___ \\_|/  .-.  \\ |   \\ | | ______ | |__) |    | |__) |/ .'   \\_|\n"
        " _   | |   _.____`. | |   | | | |\\ \\| ||______||  __ /     |  ___/ | |       \n"
        "| |__' |  | \\____) |\\  `-'  /_| |_\\   |_      _| |  \\ \\_  _| |_    \\ `.___.'\\\n"
        "`.____.'   \\______.' `.___.'|_____|\\____|    |____| |___||_____|    `.____ .'";
static char args_doc[] = "[VALUE]...";

static struct argp_option options[] = {
        { "port", 'p', "port", 0, "Port binding."},
        { "root", 'r', "root", 0, "Server root path."},
        { 0 }
};

typedef struct t_arguments
{

    u_short port;
    char * root_dir;

} arguments;

static error_t parse_opt(int key, char *arg, struct argp_state *state) {

    arguments * args = state->input;

    switch (key) {

        case 'p':
            if(isdigit(arg)) {
                args->port = atoi(arg);
            }
            break;
        case 'r':
            args->root_dir = strdup(arg);
            break;

        default: return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp1 = {
        options,
        parse_opt,
        args_doc,
        doc
};

void lua_init();

#endif //FAST_JSONRPC_JSONRPCD_H

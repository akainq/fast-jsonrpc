//
// Created by akainq on 12.10.17.
//

#ifndef FAST_JSONRPC_JSONRPCD_H
#define FAST_JSONRPC_JSONRPCD_H
#include <json-c/json.h>
#include "lua5.3/lua.h"
#include "lua5.3/lauxlib.h"
#include <argp.h>




static char logo[] = "    _____   ______     ___   ____  _____      _______     _______     ______ \n"
        "   |_   _|.' ____ \\  .'   `.|_   \\|_   _|    |_   __ \\   |_   __ \\  .' ___  |\n"
        "     | |  | (___ \\_|/  .-.  \\ |   \\ | | ______ | |__) |    | |__) |/ .'   \\_|\n"
        " _   | |   _.____`. | |   | | | |\\ \\| ||______||  __ /     |  ___/ | |       \n"
        "| |__' |  | \\____) |\\  `-'  /_| |_\\   |_      _| |  \\ \\_  _| |_    \\ `.___.'\\\n"
        "`.____.'   \\______.' `.___.'|_____|\\____|    |____| |___||_____|    `.____ .'";


void lua_init(lua_State ** L , const char * root);

#endif //FAST_JSONRPC_JSONRPCD_H

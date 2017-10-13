//
// Created by akainq on 12.10.17.
//
#define _GNU_SOURCE
#include <memory.h>
#include "include/http_handler.h"
#include "include/jsonrpcd.h"
#include <libconfig.h>
#include <event2/http.h>
#include <event2/event.h>
#include <signal.h>

const char *argp_program_version = "fast-jsonrpcd 0.01a";
const char *argp_program_bug_address = "<aka.inq@gmail.com>";

static void fatal_callback(int err){

    exit(EXIT_FAILURE);
}

void libeventlog(int severity, const char *msg)
{
    fprintf(stderr, "\tLIBEVENT: %s\n", msg);
}

static void sighandler(int signal){
    int res = signal;
}

#define NUM_THREADS 4

int main(int argc, char **argv) {


    config_t cfg;
    config_setting_t *setting;
    const char * ROOT_DIR;

    config_init(&cfg);

    const char * path = getenv("JSONRPCD_PATH");

    char *  config_path;

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

    int port;
    if (!config_lookup_int(&cfg, "dev.Network.Port", &port)){
        printf("Config error name: %d\n\n", port);
    }


    if (!config_lookup_string(&cfg, "dev.Application.root", &ROOT_DIR)){
        printf("Config error name: %s\n\n", ROOT_DIR);
    }
    evthread_use_pthreads();
    const u_short        http_port = (u_short)port;
    const char          *http_addr = hostname;
    struct evhttp *http_server = NULL;


    //event_enable_debug_mode();
    //event_set_log_callback(libeventlog);


    struct event_base * eb = event_base_new();


    http_server = evhttp_new(eb);


    evhttp_bind_socket(http_server, http_addr, http_port);
    evhttp_set_gencb(http_server, request_handler, (void *)&workqueue);
    event_set_fatal_callback(fatal_callback);

    /* Set signal handlers */
    sigset_t sigset;
    sigemptyset(&sigset);
    struct sigaction siginfo = {
            .sa_handler = sighandler,
            .sa_mask = sigset,
            .sa_flags = SA_RESTART,
    };
    sigaction(SIGINT, &siginfo, NULL);
    sigaction(SIGTERM, &siginfo, NULL);


    /* Initialize work queue. */
    if (workqueue_init(&workqueue, NUM_THREADS, ROOT_DIR)) {
        perror("Failed to create work queue");
        workqueue_shutdown(&workqueue);
        return 1;
    }


    fprintf(stderr, "%s\n", logo);
    fprintf(stderr, " \n");
    fprintf(stderr, " \n");
    fprintf(stderr, "Server started on port %d...\n", http_port);
    // eb = event_base_new();

    int ret = event_base_loop(eb,0);
              event_base_free(eb);
     /* Cya, Lua */
    free(config_path);
    evhttp_free(http_server);
    return(0);
}



void lua_init(lua_State ** L , const char * root){

    *L = luaL_newstate();
    luaL_openlibs(*L);

    char * filename_buff = calloc(256, sizeof(char));

    sprintf(filename_buff,"%s/globals.lua",root);
    int status = luaL_dofile(*L,filename_buff);
    if (status) {
        fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(*L, -1));
    }

    sprintf(filename_buff,"%s/main.lua",root);
    status = luaL_loadfile(*L,filename_buff);

    if (status) {
        fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(*L, -1));
    }

    free(filename_buff);

}

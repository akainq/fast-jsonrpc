cmake_minimum_required(VERSION 3.9)
project(fast_jsonrpc C)

set(CMAKE_C_STANDARD 11)

add_executable(fast_jsonrpc src/http_handler.c src/include/http_handler.h src/include/jsonrpcd.h src/jsonrpcd.c src/include/workqueue.h src/workqueue.c)


set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-msse3")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-msse3")

message("Searching lua library...")

find_library(LUA_LIB NAMES lua53 lua5.3 lua-5.3 lua
                PATH_SUFFIXES lib64 lib
                PATHS
                ~/Library/Frameworks
                /Library/Frameworks
                /usr/local
                /usr
                /sw
                /opt/local
                /opt/csw
                /opt
                )

find_library(LIB_EVENT NAMES libevent libevent.so
        PATH_SUFFIXES lib64 lib
        PATHS
        /usr/lib/x86_64-linux-gnu
        ~/Library/Frameworks
        /Library/Frameworks
        /opt/libevent/lib
        /opt/libevent
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        )

find_library(LIB_EVENT_PTHREADS NAMES libevent_pthreads libevent_pthreads.so
        PATH_SUFFIXES lib64 lib
        PATHS
        /usr/lib/x86_64-linux-gnu
        ~/Library/Frameworks
        /Library/Frameworks
        /opt/libevent/lib
        /opt/libevent
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        )

find_library(JSOC_C NAMES json-c
        PATH_SUFFIXES lib64 lib
        PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        )
find_library(PTHREAD pthread
        PATH_SUFFIXES lib64 lib
        PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        )
find_library(LIB_CONFIG libconfig libconfig.so
        PATH_SUFFIXES lib64 lib
        PATHS

        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        )

if(NOT LUA_LIB)
    message(FATAL_ERROR "lua library not found")
endif()


target_link_libraries(fast_jsonrpc ${LIB_EVENT})
target_link_libraries(fast_jsonrpc ${LIB_EVENT_PTHREADS})
target_link_libraries(fast_jsonrpc ${JSOC_C})
target_link_libraries(fast_jsonrpc ${LIB_CONFIG})
target_link_libraries(fast_jsonrpc ${PTHREAD})
target_link_libraries(fast_jsonrpc ${LUA_LIB})
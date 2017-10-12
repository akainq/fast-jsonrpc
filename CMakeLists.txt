cmake_minimum_required(VERSION 3.9)
project(fast_jsonrpc C)

set(CMAKE_C_STANDARD 11)

add_executable(fast_jsonrpc src/main.c)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-msse3")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-msse3")

target_link_libraries(fast_jsonrpc /opt/libevent/lib/libevent.a)
target_link_libraries(fast_jsonrpc /opt/libevent/lib/libevent_core.a)
target_link_libraries(fast_jsonrpc uuid.a)
target_link_libraries(fast_jsonrpc json-c.a)
target_link_libraries(fast_jsonrpc lua5.3.so)
#target_link_libraries(fast_jsonrpc libpq.so)
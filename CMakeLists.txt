cmake_minimum_required(VERSION 3.9)
project(fast_jsonrpc C)

set(CMAKE_C_STANDARD 11)

add_executable(fast_jsonrpc src/http_handler.c src/include/http_handler.h src/include/jsonrpcd.h src/jsonrpcd.c)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-msse3")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-msse3")

target_link_libraries(fast_jsonrpc libevent.so)
target_link_libraries(fast_jsonrpc libevent_core.so)
target_link_libraries(fast_jsonrpc uuid.so)
target_link_libraries(fast_jsonrpc json-c.so)
target_link_libraries(fast_jsonrpc lua5.3.so)
target_link_libraries(fast_jsonrpc libconfig.so)
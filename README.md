JSONRPCD LUA Service
=====================================

JSONRPCD is a fast and simple micro-framework for lua.
JSONRPC 2.0 spec implemented http://www.jsonrpc.org/specification


### Features:

**Fast** - Written on C , build with [libevent](https://github.com/libevent/libevent) and [json-c](https://github.com/json-c/json-c) library. 
23000 json-rpc calls per sec out of box.

**Simple** - JSON-RPC methods is a lua functions and hot-swapping already implemented.

**Flexibility** - embedded lua, strives to provide simple, 
flexible meta-features that can be extended as needed.

### Build process:
Prepare:
 1. You need CMake => VERSION 3.9

 2. Dependencies libraries: 
 - lua5.3.so, 
 - libevent.so, 
 - uuid.so, 
 - json-c.so, 
 - libconfig.so

Do build:
   ```shell   
   cmake .
   make
   ```



### Example:

**1.** Create first json-rpc method: 
#### NOTICE: We need to use a "jsonrpc_" prefix in lua methods declaration for protecting..

main.lua
```lua
function jsonrpc_hello_world(var1)

    local  result = {}
    result["response"] = "Hello "..var1
    return result 
end
```

**2.** Run:
#### NOTICE: You can change host, port and root dir in jsonrpcd.cfg file.
  ```shell
 
  ./fjsonrpc 
     _____   ______     ___   ____  _____      _______     _______     ______ 
   |_   _|.' ____ \  .'   `.|_   \|_   _|    |_   __ \   |_   __ \  .' ___  |
     | |  | (___ \_|/  .-.  \ |   \ | | ______ | |__) |    | |__) |/ .'   \_|
 _   | |   _.____`. | |   | | | |\ \| ||______||  __ /     |  ___/ | |       
| |__' |  | \____) |\  `-'  /_| |_\   |_      _| |  \ \_  _| |_    \ `.___.'\
`.____.'   \______.' `.___.'|_____|\____|    |____| |___||_____|    `.____ .'
  

Server started on port 5555...
   ```

**3.** Get result:
 
 request:
   ```shell
    curl -d " {"jsonrpc": "2.0", "method": "subtract", "params": ["world!"], "id": 1}" -X POST http://localhost:5555/
   ```
  
 response:
  ```shell
    {"jsonrpc": "2.0", "result": "hello world!", "id": 1}
  ```

FAST-JSONRPC LUA Service
=====================================

FAST-JSON-RPC is a fast and simple micro-framework for lua.
JSONRPC 2.0 spec implemented http://www.jsonrpc.org/specification

### Features:

**Fast** - Written on C , build with libevent and json-c library. 
23000 json-rpc calls per sec out of box.

**Simple** - JSON-RPC methods is a lua functions, having hot sourcecode swap.

**Flexibility** - embedded lua, strives to provide simple, 
flexible meta-features that can be extended as needed.


### Example:

**1.** Create first json-rpc method: 
#### NOTICE: We need to use a json-rpc prefix for lua methods.

main.lua
```lua
function jsonrpc_hello_world(var1)

    local  result = {}
    result["response"] = "Hello "..var1
    return result 
end
```

**2.** Run:
  ```shell
./fjsonrpc --port 5555  
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

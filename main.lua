--
-- Created by IntelliJ IDEA.
-- User: Aleksey Kuznetsov
-- Date: 10.10.17
-- Time: 19:18
--
-- Entery point of JSONRPC client requests
--

--@jsonrpc_method
function jsonrpc_request_handler2(a, b , c)

     local  result = {}
            result["request"] = a
            result["lua"] = "result is:"..(a..b)
            result["response"] = 2345


     --local  res = json.encode(result)

            -- con:execute(string.format([[ INSERT INTO test  (data) VALUES ('%s')]], res) )

    -- local  cur =  con:execute(string.format([[ SELECT data FROM test  LIMIT 1]]))

     --local  row = cur:fetch ({}, "a")
     --       cur:close()

     return result --row.data
end


function jsonrpc_hello_world(js)

    local  result = {}
    result["response"] = "Hello "..var1
    return result

end

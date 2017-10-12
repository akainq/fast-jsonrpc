--
-- Created by IntelliJ IDEA.
-- User: akainq
-- Date: 11.10.17
-- Time: 1:23
-- To change this template use File | Settings | File Templates.
--
package.path = package.path .. ';../?.lua'
function get_num_args(func) return debug.getinfo(_G[func]).nparams end


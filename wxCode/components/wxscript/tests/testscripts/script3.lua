-- Declares some simple test functions

TRUE = true
FALSE = false

function lua_func1(str, n)
    local i, res
    res = 0
    
    for i=1,n do          -- the fist character in a Lua string is at index 1 !!!
        res = res + string.byte(str, i)
    end

    return res
end


function lua_func2(input)
    return not input
end





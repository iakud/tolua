
function testfunc()
	-- 调用C++接口
	local a = ClassA:create()
	print(a:getMessage())
	-- 支持userdata直接get/set
	a.number = 1993
	a.string = "tolua string"
	print(a.number)
	print(a.string)
end

-- 调用堆栈
__TRACKBACK__ = function(msg)
    return debug.traceback(msg, 3)
end
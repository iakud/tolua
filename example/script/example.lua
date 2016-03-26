function createfunc()
	-- 调用C++接口
	local a = ClassA:create()
	print(a:getMessage())
end

function setfunc(a)
	-- 支持userdata直接get/set
	a.number = 1993
	a.string = "tolua string"
end

function checkfunc(a)
	assert(a.number)
	assert(a.string)
	print(a.number)
	print(a.string)
end

-- 调用堆栈
__TRACKBACK__ = function(msg)
    return debug.traceback(msg, 3)
end
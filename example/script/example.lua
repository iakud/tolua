function createfunc()
	-- 调用C++接口
	local a = ClassA:create()
	a:setCallback(function(msg)
		onCallback(a, msg)
	end)
	return a
end

function onCallback(a, msg)
	print(a:getMessage())
	print(msg)
end

function setfunc(a)
	-- 支持userdata直接get/set
	a.number = 1993
	a.string = "tolua string"
end

function getfunc(a)
	return a.number, a.string
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
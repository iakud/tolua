function create_book(name)
	local b = Book:create(name)
	b:setSellCallback(function(n)
		print(b:name().." sell: "..n)
	end)
	return b
end

function store_author(book, author)
	book.author = author
end

function load_author(book)
	assert(book.author)
	return book.author
end

function print_name(book)
	print(book:name())
end

function test_error()
	error("in test_error()")
end
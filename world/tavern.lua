render = function()
	window(40, 10, 0, 0);

	println("Hello world!")
	println("Press ESC to pause.")
	println("Press t to go to town.")
end

update = function(input)
	if (input == 't') then
		move("town")
	end
end

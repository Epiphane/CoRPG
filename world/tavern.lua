render = function()
	window(40, 10, 0, 0);

	println("Hello, "..player:get("name").."!")
	println()

	println("Press t to go to town.")
	println("Press b to fight the boss.")

	window(20, 10, 30, 0)
	println(player:get("name"))
	println()
	println("Lvl "..player:get("level"))
	println("HP: "..player:get("health"))
end

update = function(input)
	if (input == 't') then
		move("town")
	elseif (input == 'i') then
		player:info()
	end
end

depend("boss_room")
		
boss = findObject("boss")

render = function()
	window(40, 10, -15, 0);

	println("Hello, "..player:get("name").."!")
	println()

	println("Boss name: "..boss:get("name"))
	println()

	println("Press t to go to town.")
	println("Press b to fight the boss.")
	println("Press i to see boss info.")

	window(20, 10, 15, 0)
	println(player:get("name"))
	println()
	println("Lvl "..player:get("level"))
	println("HP: "..player:get("health"))
end

update = function(input)
	if (input == 't') then
		move("town")
	elseif (input == 'i') then
		boss:info()
	elseif (input == 'b') then
		move("boss_room")
	end
end

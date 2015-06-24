require "world/input"

depend("boss_room")

boss = findObject("boss")
name = Input:new()

i = ""

render = function()
	window(40, 20, -10, 0)

	println("Hooray!")
	println()
	println("You beat "..boss:get("name")..", "..player:get("name").."!")

	println()
	println()
	print("Enter your name: "..name.val)
end

update = function(input)
	i = input
	if not name.done then
		name:update(input)
	end
end

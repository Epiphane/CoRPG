require "world/input"

depend("boss_room")

boss = findObject("boss")
name = Input:new()

i = ""

leaders = {}

render = function()
	window(40, 20, 0, 0)

	println("Hooray!")
	println()
	println("You beat "..boss:get("name")..", "..player:get("name").."!")

	println()
	println()

	println()
	println("Current Leaders:")
	for n, name in ipairs(leaders) do
		println(n..". "..name)
	end

	if name.done then
		println()
		println()
		println("Press any key to continue...")
	end

	cursor(2, 4)
	print("Enter your name: "..name.val)
end

update = function(input)
	if not name.done then
		name:update(input)

		if name.done then
			table.insert(leaders, 1, name.val.." beat "..boss:get("name"))
		end
	else
		move("boss_create")
	end
end

saveData = function()
	for l = 1, #leaders do
		writeln(leaders[l])
	end
end

loadData = function(input)
	for n, line in ipairs(input) do
		table.insert(leaders, line)
	end
end

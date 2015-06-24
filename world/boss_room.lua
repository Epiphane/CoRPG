boss = findObject("boss")

getBars = function(max, inc)
	top = ""
	mid = ""
	bot = ""
	i = 0
	while (i < max) do
		top = top.."-"
		mid = mid.." "
		bot = bot.."-"
		i = i + inc
	end

	return top, mid, bot
end

printBossBar = function(max, inc)
	top, mid, bot = getBars(max, inc)

	cursor(28, 0)
	println(top.."\\")
	cursor(28, 1)
	println(mid.."|")
	cursor(28, 2)
	println(bot.."/")
end

printPlayerBar = function(max, inc)
	top, mid, bot = getBars(max, inc)

	x = -28 - string.len(top)
	y = -1

	cursor(x, y-2)
	println("/"..top)
	cursor(x, y-1)
	println("|"..mid)
	cursor(x, y)
	println("\\"..bot)
end

render = function()
	window(80, 20, 0, 0)
	printBossBar(tonumber(boss:get("health")), 3)
	printPlayerBar(tonumber(player:get("health")), 3)
	
	window(30, 6, -25, -7)
	println(boss:get("name"))
	println("HP: "..boss:get("health").."/"..boss:get("maxhealth"))

	window(30, 6, 25, 7)
	println(player:get("name"))
	println("HP: "..player:get("health").."/"..player:get("maxhealth"))
end

update = function(input)
	if (input == 't') then
		move("tavern")
	end
end

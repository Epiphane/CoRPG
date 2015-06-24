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

Battle = { 
	p1 = nil, 
	p2 = nil
}

function Battle:new (p1, p2)
	o = { p1 =  p1, p2 = p2 }
	setmetatable(o, self)
	self.__index = self
	return o
end

function Battle:render ()
	window(80, 20, 0, 0)
	printBossBar(tonumber(self.p2:get("health")), 3)
	printPlayerBar(tonumber(self.p1:get("health")), 3)
	
	window(30, 6, -25, -7)
	println(self.p2:get("name"))
	println("HP: "..self.p2:get("health").."/"..self.p2:get("maxhealth"))

	window(30, 6, 25, 7)
	println(self.p1:get("name"))
	println("HP: "..self.p1:get("health").."/"..self.p1:get("maxhealth"))
end

function Battle:update(input)
	if (input == ' ') then
		self.p2:set("health", self.p2:get("health") - 5)
	end
end

render = function()
	window(60, 10, -10, 0)

--	player = Character:new()
--	player:setProperty("something", "something else")

--	println("Player info: "..player:getProperty("something"))

	println("You're in town")
	println("Press t to go back to tavern")
end

update = function(input)
	if (input == 't') then
		move("tavern")
	end
end

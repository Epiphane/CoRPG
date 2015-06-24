require "world/battle"

boss = findObject("boss")

battle = Battle:new(player, boss)

render = function()
	battle:render()
end

update = function(input)
	if (input == 't') then
		move("tavern")
	else
		battle:update(input)
	end
	
	if (tonumber(boss:get("health")) <= 0) then
		move("boss_victory")
	end
end

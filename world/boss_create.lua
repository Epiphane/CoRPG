require "world/input"

depend("boss_room")

boss = findObject("boss")

boss_name   = Input:new()
boss_health = NumInput:new()

render = function()
	window(40, 20, 0, 0)

	println("Now it's your turn!")
	print("Enter new boss' name: "..boss_name.val)
	
	if boss_name.done then
		println()
		println()
		print("Enter new boss' health: "..boss_health.val)
	end

	if boss_health.done then
		println("Press any key to continue...")
	end
end

update = function(input)
	if not boss_name.done then
		boss_name:update(input)
	elseif not boss_health.done then
		boss_health:update(input)
	else
		boss:set("name", boss_name.val)
		boss:set("health", boss_health.val)

		move("tavern")
	end
end

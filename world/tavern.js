depend("boss_room");

var boss = new GameObject("boss", {
	health: 2200,
	max_health: 2200,
	level: 120
});

render = function() {
	window(40, 10, -15, 0);

	println("Hello, " + player.get("name") + "!")
	println()

	println("Boss name: " + boss.get("name"))
	println()

	println("Press t to go to town.")
	println("Press b to fight the boss.")
	println("Press i to see boss info.")

	window(20, 10, 15, 0)
	println(player.get("name"))
	println()
	println("Lvl " + player.get("level"))
	println("HP: " + player.get("health"))
};

update = function(input) {
	if (input == "i")
		boss.info();
	else if (input == "t")
		move("town");
	else if (input == "b")
		move("boss_room");
}

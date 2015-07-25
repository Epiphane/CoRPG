depend("boss_room");

var _moves = require('./moves.js').Moves;
var Battle = require("./battle.js").Battle;

var boss = new GameObject("boss", {
	health: 2200,
	max_health: 2200,
	level: 120
});

render = function() {
	window(40, 10, 0, 0);

	println("Hello, " + player.get("name") + "!")
	println()

	println("Press t to go to town.")
};

update = function(input) {
	if (input == "i")
		boss.info();
	else if (input == "t")
		move("town");
	else if (input == "b")
		move("boss_room");
};
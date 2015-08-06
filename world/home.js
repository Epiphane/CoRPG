depend("boss_room");

direction('<', 'Market',     'market');
direction('v', 'Your House', 'house');
direction('>', 'Gardens',    'gardens');

var _moves = require('./moves.js').Moves;
var Battle = require("./battle.js").Battle;

var boss = new GameObject("boss", {
	health: 2200,
	max_health: 2200,
	level: 120
});

render = function() {
	window(40, 12, 0, 0);

	println('~The town of Weebly~');
	println();

	println("Hello, " + player.get("name") + "!")
	println()

	println('As you can see, the evil boss')
	println(boss.get('name') + ' has been ravaging Weeblia..');
	println()
	println('Please save our land!');
};

update = function(input) {
	if (input == "b")
		move("boss_room");
};
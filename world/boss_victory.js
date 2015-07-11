var inputs = require("./input.js");
var Input = inputs.Input;

depend("boss_room");

var boss = new GameObject("boss", {});
var name = new Input();

var board = new GameObject("board", {
	level: 1
});

var insertNewWinner = function(name) {
	var update = {};
	update[board.get("level") + "_winner"] = name;
	update.level = board.get("level") + 1;

	board.set(update);
}

render = function() {
	window(40, 20, 0, 0);

	println("Hooray!");
	println("You beat " + boss.get("name") + ", " + player.get("name") + "!");

	println();
	println();

	println();
	println("Current Leaders:");
	for (var i = 1; i < board.get("level"); i ++) {
		println(i + ". " + board.get(i + "_winner"));
	}

	if (name.done) {
		println();
		println();
		println("Press any key to continue...");
	}

	cursor(2,6);
	print("Enter your name: " + name.val);

}

update = function(input) {
	if (!name.done) {
		name.update(input);

		if (name.done) {
			insertNewWinner(name.val + " beat " + boss.get("name"));
		}
	}
	else {
		move("boss_create");
	}
}

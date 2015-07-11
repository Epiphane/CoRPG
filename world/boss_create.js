var inputs = require("./input.js");

depend("boss_room");

var boss = new GameObject("boss", {});
var boss_name = new inputs.Input();
var boss_health = new inputs.NumInput();

render = function() {
	window(40, 20, 0, 0);

	println("Now it's your turn!")
	print("Enter new boss' name: " + boss_name.val);

	if (boss_name.done) {
		println();
		println();
		print("Enter new boss' health: " + boss_health.val);
	}

	if (boss_health.done) {
		println();
		println("Press any key to continue...");
	}
}

update = function(input) {
	if (!boss_name.done) {
		boss_name.update(input);
	}
	else if (!boss_health.done) {
		boss_health.update(input);
	}
	else {
		boss.set({
			name: boss_name.val,
			health: boss_health.val
		});

		move("tavern");
	}
}

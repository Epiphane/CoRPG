var inputs = require("./input.js");

depend("boss_room");

var boss = new GameObject("boss", {});
var boss_name = new inputs.Input();
var boss_health = new inputs.NumInput();

var uh_oh = false;
if (boss.get('health') > 0) {
	// Someone else made the new boss!
	uh_oh = true;
}

render = function() {
	window(40, 20, 0, 0);
	if (uh_oh) {
		println("UH OH");

		println("Someone made the boss already! Sending you back home...");
		println("Press any key to continue...");
		return;
	}

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
	if (uh_oh) {
		move("home");
	}
	else if (!boss_name.done) {
		boss_name.update(input);
	}
	else if (!boss_health.done) {
		boss_health.update(input);
	}
	else {
		boss.set({
			name: boss_name.val,
			health: boss_health.val,
			max_health: boss_health.val
		});

		move("home");
	}
}

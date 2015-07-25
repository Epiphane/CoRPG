var Battle = require("./battle.js").Battle;

var boss = new GameObject("boss", {
	health: 2200,
	max_health: 2200,
	level: 120
});

boss.image = 'boss.txt';
player.image = 'player.txt';

battle = new Battle(player, boss);

render = function() {
	battle.render();
};

update = function(input) {
	if (battle.win) {
		move("boss_victory");
	}
	else if (battle.lose) {
		move('home');
	}

	if (input == "t")
		move("home");
	else
		battle.update(input);

	if (boss.get("health") <= 0)
		move("boss_victory");
};

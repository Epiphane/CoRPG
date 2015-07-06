var Battle = require("./battle.js").Battle;

var boss = new GameObject("boss", {
	health: 2200,
	max_health: 2200,
	level: 120
});

battle = new Battle(player, boss);

render = function() {
	battle.render();
};

update = function(input) {
	if (input == "t")
		move("tavern");
	else
		battle.update(input);

	if (boss.get("health") <= 0)
		move("boss_victory");
};

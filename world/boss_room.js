var Battle = require("./battle.js").Battle;

var boss = new GameObject("boss", {
	health: 2200,
	max_health: 2200,
	level: 120
});

battle = new Battle(player, boss);

render = function() {
	battle.renderScene();

	drawImage(1, -10, 'player.txt');
	/*draw(1, -10, 
		'         ______', 
		'      .--      -', 
		'     /          \\___', 
		'     |          |', 
		'     (           )', 
		'      \\---------',
		'     ----\\      \\',
		'    /   / \\      \\');*/

	battle.renderPlayers();
};

update = function(input) {
	if (input == "t")
		move("home");
	else
		battle.update(input);

	if (boss.get("health") <= 0)
		move("boss_victory");
};

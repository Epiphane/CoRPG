depend("boss_room");

direction('<', 'Home', 'home');

var serenity = parseInt(player.get('serenity') || "0") + 1;
player.set({
	serenity: serenity
});

render = function() {
	window(40, 12, 0, 0);

	println('~The gardens of Weebly~');
	println();

	println("Aww, how nice. You feel more serene..")
};

update = function(input) {
};
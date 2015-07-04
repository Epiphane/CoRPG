var thing = "";

render = function() {
	window(24, 10);

	println("This is the render!");
	println("Hello, " + player.name);
};

update = function(ch) {
	thing = ch;

	if (thing == "i") {
		player.info();
	}
}

render = function() {
	window(60, 10, -10, 0)

	println("You're in town")
	println("Press t to go back home")
}

update = function(input) {
	if (input == "t")
		move("home");
}

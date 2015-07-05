render = function() {
	window(60, 10, -10, 0)

	println("You're in town")
	println("Press t to go back to tavern")
}

update = function(input) {
	if (input == "t")
		move("tavern");
}

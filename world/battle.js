exports.Battle = Battle = function(p1, p2) {
	this.p1 = p1;
	this.p2 = p2;
};

function makeBars(max, current) {
	var hpPerCh = max / 30;

	return {
		top: Array(Math.ceil(current / hpPerCh) + 1).join("-"),
		mid: Array(Math.ceil(current / hpPerCh) + 1).join(" ")
	};
};

Battle.prototype.renderP2Bar = function(max, current) {
	var bars = makeBars(max, current);

	cursor(28, 1);
	println(bars.top + "\\");
	cursor(28, 2);
	println(bars.mid + "|");
	cursor(28, 3);
	println(bars.top + "/");
};

Battle.prototype.renderP1Bar = function(max, current) {
	var bars = makeBars(max, current);

	var x = -28 - bars.top.length;
	var y = -2;

	cursor(x, y-2);
	println("/" + bars.top);
	cursor(x, y-1);
	println("|" + bars.mid);
	cursor(x, y);
	println("\\" + bars.top);
};

thing = ""

Battle.prototype.render = function() {
	window(80, 20);

	this.renderP2Bar(this.p2.get("max_health"), this.p2.get("health"));
	this.renderP1Bar(this.p1.get("max_health"), this.p1.get("health"));

	window(30, 6, -25, -7);
	println(this.p2.get("name"));
	println("HP: " + this.p2.get("health") + "/" + this.p2.get("max_health"))

	window(30, 6, 25, 7);
	println(this.p1.get("name"));
	println("HP: " + this.p1.get("health") + "/" + this.p1.get("max_health"))
};

Battle.prototype.update = function(input) {
	thing = input;
	if (input == " ") {
		thing = this.p2.get("health") - 10
		this.p2.set("health", this.p2.get("health") - 10);
	}
};

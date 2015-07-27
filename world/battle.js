var Moves = require('./moves.js').Moves;

exports.Battle = Battle = function(p1, p2) {
	this.p1 = p1;
	this.p2 = p2;
	this.p1_action = '';
	this.p2_action = '';
	this.p1_action_tag = '';
	this.p2_action_tag = '';

	this.lose         = false;
	this.win          = false;
	this.choosingMove = false;
};

function makeBars(max, current) {
	var hpPerCh = max / 30;

	return {
		top: Array(Math.ceil(current / hpPerCh) + 1).join("_"),
		mid: Array(Math.ceil(current / hpPerCh) + 1).join(" ")
	};
};

Battle.prototype.renderP2Bar = function(max, current) {
	var bars = makeBars(max, current);

	cursor(28, 1);
	println(bars.top + " ");
	cursor(28, 2);
	println(bars.top + "|");
};

Battle.prototype.renderP1Bar = function(max, current) {
	var bars = makeBars(max, current);

	var x = -28 - bars.top.length;
	var y = -2;

	cursor(x, y-2);
	println(" " + bars.top);
	cursor(x, y-1);
	println("|" + bars.top);
};

Battle.prototype.render = function() {
	if (this.p2_action) {
		if (this.p2_action_tag)
			window(60, 4, 0, -12);
		else
			window(60, 5, 0, -13);

		println(this.p2_action);
		println(this.p2_action_tag);
	}
	if (this.p1_action) {
		if (this.p1_action_tag)
			window(60, 5, 0, 12);
		else
			window(60, 4, 0, 12);

		println(this.p1_action);
		println(this.p1_action_tag);
	}

	window(80, 20);

	if (this.p1.image) {
		drawImage(1, -10, this.p1.image);
	}
	if (this.p2.image) {
		drawImage(-30, 1, this.p2.image);
	}

	this.renderP2Bar(this.p2.get("max_health"), this.p2.get("health"));
	this.renderP1Bar(this.p1.get("max_health"), this.p1.get("health"));

	window(30, 6, -25, -7);
	println(this.p2.get("name"));
	println("HP: " + this.p2.get("health") + "/" + this.p2.get("max_health"))

	window(30, 6, 25, 7);
	println(this.p1.get("name"));
	println("HP: " + this.p1.get("health") + "/" + this.p1.get("max_health"))

	if (this.choosingMove) {
		window(30, 10, -15, 5);

		for (var i = 0; i < this.p1_moves.length; i ++) {
			if (this.cursor === i) {
				print('->')
			}
			else {
				print('  ');
			}
			println(' ' + this.p1_moves[i].name);
		}

		window(60, 4, 0, 18);
		println("Press space to choose");
	}
	else {
		window(60, 4, 0, 18);
		println("Press space to continue");
	}
};

Battle.prototype.choose = function() {
	this.choosingMove = false;
};

Battle.prototype.update = function(input) {
	if (!this.choosingMove) {
		this.choosingMove = true;

		this.cursor = 0;

		this.p1_action = '';
		this.p2_action = '';

		this.p1_moves = Moves(this.p1);
		this.p2_moves = Moves(this.p2);

		// Add runaway option for player
		this.p1_moves.push({
			name: 'Run Away',
			action: 'flee'
		});
	}
	else {
		if (input == "up") {
			this.cursor --;
		}
		else if (input == "down") {
			this.cursor ++;
		}
		this.cursor = this.cursor % this.p1_moves.length;
		if (this.cursor < 0) {
			this.cursor += this.p1_moves.length;
		}

		if (input == ' ') {
			var action = this.p1_moves[this.cursor];

			this.p1_action_tag = '';
			this.p1_action = this.p1.get('name') + ' uses ' + action.name;

			if (typeof(action.action) === 'object') {
				this.p1.actOn(this.p2, action.action);
			}
			else if (typeof(action.action) === 'function') {
				action.action(this.p1, this.p2);
			}
			else {
				(action.object || this)[action.action](this.p1, this.p2);
			}

			this.choosingMove = false;
		}
	}
};

Battle.prototype.flee = function(p1, p2) {
	if (p1.get('level') > p2.get('level')) {
		this.lose = true;
	}
	else if (Math.random() * p2.get('level') < p1.get('level')) {
		this.lose = true;
	}
	else {
		this.p1_action_tag = 'But it failed!';
	}
};

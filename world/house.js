direction('^', 'Town Square', 'home');

var inputs = require("./input.js");
var writing = null;

var notebook = new GameObject('notebook', { level: 1 });
if (notebook.get('new')) {
	notebook.set({
		entries: 0
	});
}

render = function() {
	window(40, 12, 0, 0);

	println('~Your House~');
	println();

	println('It\'s pretty quaint in here.');
	println('Not a whole lot to do though...');
	println();

	var entries = notebook.get('entries');
	println('Your notebook has ' + entries + ' entries in it.');
	if (entries > 0) {
		println('The first one reads: ');
		println(notebook.get('entry_' + entries));
	}

	println();
	if (!writing) {
		println('Press n to write something down');
	}
	else {
		println(writing.val);
	}
};

update = function(input) {
	if (writing) {
		writing.update(input);

		if (writing.done) {
			var entry = parseInt(notebook.get('entries')) + 1;

			var props = { entries: entry };
			props['entry_' + entry] = writing.val;
			notebook.set(props);

			writing = false;
		}
	}
	else {
		if (input === 'n') {
			writing = new inputs.Input();;
		}
	}
};
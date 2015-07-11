exports.Input = Input = function() {
	this.done = false;
	this.val = "";
};

Input.prototype.update = function(input) {
	if (!this.done) {
		if (input == "\b") {
			if (this.val.length > 0) {
				this.val = this.val.substring(0, this.val.length - 1);
			}
		}
		else if (input == "\n") {
			this.done = true;
		}
		else {
			this.val += input;
		}
	}
};

exports.NumInput = NumInput = function() {
	this.done = false;
	this.val = "";
};

NumInput.prototype.update = function(input) {
	if (!thisdone) {
		if (input == "\b") {
			if (this.val.length > 0) {
				this.val = this.val.substring(0, this.val.length - 1);
			}
		}
		else if (input == "\n") {
			this.done = true;
		}
		else if (!isNan(parseInt(input, 10))) {
			this.val += input;
		}
	}
};

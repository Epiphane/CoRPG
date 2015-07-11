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
	this.val = 0;
};

NumInput.prototype.update = function(input) {
	if (!this.done) {
		if (input == "\b") {
			if (this.val >= 10) {
				this.val = Math.floor(this.val / 10);
			}
			else {
				this.val = 0;
			}
		}
		else if (input == "\n") {
			this.done = true;
		}
		else if (!isNaN(parseInt(input, 10))) {
			this.val = this.val * 10 + parseInt(input, 10);
		}
	}
};

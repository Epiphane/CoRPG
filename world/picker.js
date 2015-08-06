function Picker(options) {
   this.options = options.options || options;
   this.cursor = 0;
   this.back = options.back;

   if (this.back) {
      this.options.push({});
   }

   this.renderfunc = options.render || function(option) {
      println(option.name || option);
   }

   this.onChoose = options.onChoose;

   this.chosen  = false;
}

Picker.prototype.render = function() {
   for (var i = 0; i < this.options.length; i ++) {
      if (this.cursor === i) {
         print('-> ')
      }
      else {
         print('   ');
      }
      if (this.back && i === this.options.length - 1) {
         println('Back');
      }
      else {
         this.renderfunc(this.options[i], this.cursor === i);
      }
   }
};

Picker.prototype.update = function(input) {
   if (this.chosen !== false) {
      return;
   }

   if (input === "up") {
      this.cursor --;
      if (this.cursor < 0) {
         this.cursor += this.options.length;
      }
   }
   else if (input === "down") {
      this.cursor ++;
      if (this.cursor >= this.options.length) {
         this.cursor -= this.options.length;
      }
   }
   else if (input === ' ' || input === '\n') {
      this.chosen = this.cursor;

      if (this.back && this.chosen === this.options.length - 1) {
         this.back();
      }
      else if (this.onChoose) {
         this.onChoose(this.options[this.chosen], this.chosen);
      }
   }
};

exports.Picker = Picker;
function Picker(options) {
   this.options = options;
   this.cursor = 0;

   this.chosen  = false;
}

var last = '';
Picker.prototype.render = function() {
   for (var i = 0; i < this.options.length; i ++) {
      if (this.cursor === i) {
         print('->')
      }
      else {
         print('  ');
      }
      println(' ' + (this.options[i].name || this.options[i]));
   }
   println(last);
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
   }
};

exports.Picker = Picker;
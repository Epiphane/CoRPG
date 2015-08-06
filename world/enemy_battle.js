var Battle = require("./battle.js").Battle;

var enemy = new GameObject(args.enemy.name, args.enemy);

enemy.image = args.enemy.name + '.txt';
player.image = 'player.txt';

battle = new Battle(player, enemy);

var intro = false;

render = function() {
   if (!intro) {
      window(50, 5);

      println('Oh no! ' + enemy.get('name') + ' appeared!');
   }
   else {
      battle.render();
   }
};

update = function(input) {
   if (!intro) {
      intro = true;
      return;
   }

   if (battle.win) {
      move(args.win, { won: true });
   }
   else if (battle.lose) {
      move(args.lose, { lost: true });
   }

   if (input == "t")
      move(args.lose, { lost: true });
   else
      battle.update(input);

   if (enemy.get("health") <= 0)
      move(args.win, { won: true });
};

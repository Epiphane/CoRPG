if (!args.won && Math.random() < 0.3) {
   move('enemy_battle', {
      enemy: {
         name: 'Mushroom',
         health: 30,
         max_health: 30,
         level: 4
      },
      win: 'home_fields',
      lose: 'home_fields'
   });
// }
}
else {
   direction('v', 'Home', 'home');
}

render = function() {
   window(50, 20)

   println('~ A Peaceful Meadow ~')
   println();

   draw(0, -4, '      O     O      ',
               '      |     |      ',
               '      |     |      ');
}

update = function(input) {

}
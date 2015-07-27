var Picker = require('./picker.js').Picker;

direction('>', 'Home', 'home');

var shops = new Picker([
   'Weapons Emporium', 
   'Bottles & Draughts',
   'Scrolls for Days'
]);

var dealer_info = {
   health: 10000,
   max_health: 10000,
   level: 120,
   class: 'NPC',
   funds: 2500
};

var weapons_dealer = new GameObject("weapons dealer", dealer_info);
var potions_dealer = new GameObject("potions dealer", dealer_info);
var scrolls_dealer = new GameObject("scrolls dealer", dealer_info);

var excalibur = new GameObject("excalibur", {
   level: 70,
   class: 'weapon',
   attack: 42
});

weapons_dealer.actOn(excalibur, {
   method: "own"
});

var currentShop = false;

render = function() {
   window(60, 20);

   println('Welcome to the market!');

   println();
   if (currentShop === false) {
      println('All kinds of wares here, check out a shop!')

      println();
      shops.render();
   }
   else {

   }

if (excalibur.get('new')) {
   println("oidhniouwqfhniquwf");
}

   drawImage(0, -7, 'market.txt');
};

update = function(input) {
   if (currentShop === false) {
      shops.update(input);

      currentShop = shops.chosen;
   }
};
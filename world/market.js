var Picker = require('./picker.js').Picker;
var Commerce = require('./commerce.js');

direction('>', 'Home', 'home');

var dealer_info = {
   health: 10000,
   max_health: 10000,
   level: 120,
   class: 'NPC',
   gold: 2500
};

var dealers = [
   new GameObject("weapons dealer", dealer_info),
   new GameObject("potions dealer", dealer_info),
   new GameObject("scrolls dealer", dealer_info)
];

var greetings = [
   'Fresh from battle again?',
   'Eh. What brings you back?',
   'Oh boy! I love customers!'
];

var Items = require('market_items.js');

function createAllItems(collection, dealer) {
   for (var name in collection) {
      var item = new GameObject(name, collection[name]);
      if (item.get('new')) {
         dealer.actOn(item, { method: "own" });
      }
   }
}

createAllItems(Items.weapons, dealers[0]);
createAllItems(Items.potions, dealers[1]);
createAllItems(Items.scrolls, dealers[2]);

var currentShop = false;
var selector = false;
var message = '';

function shopAction(action) {
   if (action === 'Buy') {
      var wares = dealers[currentShop].get('possessions');

      selector = new Picker({
         options: wares,
         back: function() {
            chooseShop("", currentShop);
         },
         render: function(ware, selected) {
            println(ware.get('name') + ' ' + Commerce.getPrice(ware, Commerce.Gold).str);
            if (selected) {
               println('     Level:  ' + ware.get('level'));
               println('     Attack: ' + ware.get('attack'));
               println('     Rarity: ' + ware.get('rarity'));
            }
         },
         onChoose: function(item, i) {
            if (player.get('gold') && parseInt(player.get('gold')) >= Commerce.getPrice(item, Commerce.Gold).amount) {
               player.actOn(item, {
                  method: "buy",
                  price: {
                     type: Commerce.Gold,
                     amount: Commerce.getPrice(item).amount
                  }
               });

               wares.splice(i, 1);
            }
            else {
               message = 'You don\'t have enough gold for this!';
               selector.chosen = false;
            }
         }
      });
   }
   else if (action === 'Sell') {

   }
   else {
      selector = new Picker({
         options: [
            'Weapons Emporium', 
            'Bottles & Draughts',
            'Scrolls for Days'
         ], 
         onChoose: chooseShop
      });
   }
}

function chooseShop(name, shop) {
   currentShop = shop;

   selector = new Picker({
      options: [
         'Buy'/*, 'Sell'*/, 'Leave'
      ],
      onChoose: shopAction
   });
}

render = function() {
   if (!selector) {
      selector = new Picker({
         options: [
            'Weapons Emporium', 
            'Bottles & Draughts',
            'Scrolls for Days'
         ], 
         onChoose: chooseShop
      });
   }

   window(60, 20);

   println('Welcome to the market!');
   println('You have ' + player.get('gold') + ' gold.');

   println();
   if (currentShop === false) {
      println('All kinds of wares here, check out a shop!')
   }
   else {
      println(greetings[currentShop]);
   }

   if (message) {
      println(message);
      println();
      message = '';
   }

   println();
   selector.render();

   drawImage(0, -7, 'market.txt');
};

update = function(input) {
   if (selector) {
      selector.update(input);
   }
};
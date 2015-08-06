exports.Gold = 'gold';

exports.getPrice = function(item, currency) {
   var amount = item.get('level') * item.get('rarity');
   return {
      amount: amount,
      str: amount + ' ' + currency
   };
};
exports.Moves = function Moves(object) {
    var options = [];

    // Normal attack
    options.push({
        name: 'Attack',
        action: {
            method: "attack",
            damage: object.get('level')
        }
    });

    return options
};
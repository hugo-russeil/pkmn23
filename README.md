# PoKeMoN Card Game

The objective of this project is to create a Pokemon-inspired card game for 2 players, either human or AI, with a console interface.

## Specifications

- **Pokemon**: The game features Pokemon with various attributes such as name, height, weight, health points (HP), attack, defense, and type. The types include Normal, Water, Fire, Plant, Electric, Ice, Poison, Fighting, Psychic, Ground, Rock, Ghost, Bug, Flying and Dragon. Each type has specific characteristics and interactions with other types.

- **Trainers**: Trainers have a name, level, points, and a team of 6 Pokemon. They can capture or transfer Pokemon, and engage in battles. The level of a trainer increases based on the points earned through various actions.

- **Battles**: Battles occur between Pokemon from the opposing trainers' teams. The Pokemon's attributes, such as health points attack, defense and speed, determine the outcome of the battle. The battle starts with each trainer having a team of 6 Pokemon, and continue until all Pokemon from one trainer's team are knocked out.. The trainer withe the highest level chooses a pokemon to send to battle first, followed by the second player. The fastest pokemon attacks his opponent n times, n being his speed divided by his opponent's, and the slowest attacks his opponent once, until a pokemon is knocked out. After each attack, the defending pokemon takes damages, defined by :  
Math block:
$$
(((((2*50/5)+2)*50*(Atk/Def))/50)+2)*random*STAB*type
$$


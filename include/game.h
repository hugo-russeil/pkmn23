#ifndef GAME_H
#define GAME_H
#include "trainer.h"
#include "pokemon.h"

#include <vector>

class Game
{
public:
    /**
     * @brief Game the constructor of the game class
     */
    Game();
    /**
     * @brief gameLoop
     */
    void gameLoop();
    /**
     * @brief displayBattle displays on the console the pokemons names, hp and max hp, and sprites
     * @param player1 the first contestant in the battle
     * @param player2 the second contestant in the battle
     */
    void displayBattle(Trainer* player1, Trainer* player2);
    /**
     * @brief updateDisplayBattle updtates the console lines displaying the hp of the pokemons
     * @param player1 the first contestant in the battle
     * @param player2 the second contestant in the battle
     */
    void updateDisplayBattle(Trainer* player1, Trainer* player2);
    /**
     * @brief redrawPokemon redraws the players's pokemons's sprites
     * @param player1 the first contestant in the battle
     * @param player2 the second contestant in the battle
     */
    void redrawPokemon(Trainer* player1, Trainer* player2);
    /**
     * @brief attack method used to make a trainer's pokemon attack another trainer's pokemon
     * @param attacker the trainer of the attacking pokemon
     * @param defender the trainer of the defending pokemon
     */
    void attack(Trainer* attacker, Trainer* defender);
    /**
     * @brief gameMenu displays a game menu, in which the player can set up his game
     * @param selected the index of the selected option, initialy set to zero
     * @return the selected option on the menu
     */
    int gameMenu(int selected = 0);

    /**
     * @brief selectMove allows the player to select a move
     * @param pokemon the pokemon that is making the move
     * @return the selected move
     */
    Move selectMove(Pokemon &pokemon);
};



extern Trainer *player1, *player2;

#endif // GAME_H

#ifndef TRAINER_H
#define TRAINER_H

#include <iostream>
#include <vector>
#include "pokemon.h"

class Trainer
{
    /**
     * @brief itsName the trainer's name
     */
    std::string itsName;
    /**
     * @brief itsLvl the trainer's level
     */
    int itsLvl;
    /**
     * @brief itsXp the trainer's xp
     */
    int itsXp;
    /**
     * @brief itsTeam the trainer's pokemon team
     */
    std::vector<Pokemon*> itsTeam;
    /**
     * @brief isHuman attribute defining if the trainer is human or a bot
     */
    bool isHuman;
public:
    /**
     * @brief Trainer the constructor of the trainer
     * @param name the trainer's name
     */
    Trainer(std::string name);
    //getters
    /**
     * @brief getItsName returns the trainer's name
     * @return the trainer's name
     */
    std::string getItsName();
    /**
     * @brief getItsLevel returns the trainer's level
     * @return the trainer's level
     */
    int getItsLevel();
    /**
     * @brief getItsLevel returns the trainer's xp
     * @return the trainer's xp
     */
    int getItsXp();
    /**
     * @brief getItsTeam returns the trainer's team
     * @return the trainer's team
     */
    std::vector<Pokemon*> getItsTeam();
    /**
     * @brief getIsHuman used to know if a trainer is human
     * @return true if the trainer is human, false if he is a bot
     */
    bool getIsHuman();
    /**
     * @brief getItsAvgTeamSpeed return the average speed of the pokemons in the trainer's team
     * @return the average speed of the pokemons in the trainer's team
     */
    int getItsAvgTeamSpeed();

    //setters
    /**
     * @brief setIsHuman set the value of isHuman
     * @param isHuman a boolean, indicating if the trainer is a human player
     */
    void setIsHuman(bool isHuman);
    /**
     * @brief gainXp adds xp to a trainer
     * @param xp the amount of xp to add to the trainer
     */
    void gainXp(int xp);
    /**
     * @brief setLevel sets the trainer's level
     * @param lvl the level to give to the trainer
     */
    void setLevel(int lvl);
    /**
     * @brief looseXp decreases a trainer's xp
     * @param xp the amount of xp to take away from the trainer
     */
    void looseXp(int xp);
    //display
    /**
     * @brief displayAvgTeamSpeed displays the average speed of the trainer's pokemons in the console
     */
    void displayAvgTeamSpeed();
    /**
     * @brief displayAvgTeamSpeedForType displays the average speed of the trainer's pokemons of a defined type in the console
     * @param type the type for which the average speed should be displayed
     */
    void displayAvgTeamSpeedForType(std::string type);
    /**
     * @brief compareTrainers compares two trainers based on various statistics of their pokemons and themselves, and displays in the consloe the comparaisons and the most likely winner of the upcomming battle
     * @param defender the trainer to compare the trainer on which the method is used to
     */
    void compareTrainers(Trainer* defender);
    /**
     * @brief catchPokemon adds a pokemon to the team of the trainer
     * @param nb the pokedex number of the pokemon to be added
     */
    void catchPokemon(int nb);
    /**
     * @brief transferPokemon opens a menu in which the trainer can transfer a pokemon of his team for it to be replaced by another pokemon choosed randomly at the cost of 2 xp
     * @return true if the player has enough xp to transfer a pokemon, false if he doesn't
     */
    bool transferPokemon();
    /**
     * @brief choosePokemon opens a menu in which the trainer can choose a pokemon
     */
    void choosePokemon();
    /**
     * @brief botChoosePokemon method that lets a bot trainer choose a pokemon to be sent into battle
     * @param opponentsPokemon the bot's opponent's pokemon
     * @param behaviour the behaviour set for the bots, that will determine the degree of reflection in the bot's choice
     */
    void botChoosePokemon(Pokemon* opponentsPokemon, int behaviour);
    /**
     * @brief sendToBattle sends a pokemon to battle
     * @param teamRank the rank in the team vector of the pokemon to be sent into battle
     */
    void sendToBattle(int teamRank);
    /**
     * @brief teamMenu opens a menu in which the pokemons of the player team are displayed, for him to choose one
     * @param selected the rank of the selected pokemon, initialy set to zero
     * @return the selected pokemon
     */
    int teamMenu(int selected = 0);
    /**
     * @brief interBattleMenu opens a menu in which the trainer can choose to transfer a pokemon, or to go into battle
     * @param selected the rank of the selected option, initialy set to zero
     * @return the selected option
     */
    int interBattleMenu(int selected = 0);
    /**
     * @brief hasTrainerLost determines if a player has lost the game by having all his pokemons defeated
     * @return true if all the player's pokemon are fainted, false if they aren't
     */
    bool hasTrainerLost();
    /**
     * @brief healTeam heals all of trainer's pokemons that aren't fainted by setting their hp to their max hp
     */
    void healTeam();
    /**
     * @brief saveTrainer saves a trainer's informations and pokemons in a text file
     */
    void saveTrainer();
    /**
     * @brief loadTrainer loads a previously saved trainer's profile from a save file
     * @param playerName the trainer's name
     * @return an object of the trainer class, of which every attribute has been set according to the informations in the save file
     */
    Trainer *loadTrainer(std::string playerName);
};

#endif // TRAINER_H

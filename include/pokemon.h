#ifndef POKEMON_H
#define POKEMON_H
#if defined(WIN32)
#include <Windows.h>
#define CLEAR system("cls")
#elif __unix__
#define CLEAR system("clear")
#endif

#include <iostream>

class Pokemon
{
protected:
    /**
     * @brief itsNumber the pokemon's pokedex number
     */
    int itsNumber;
    /**
     * @brief itsName the pokemon's name
     */
    std::string itsName;
    /**
     * @brief itsType the pokemon's type
     */
    std::string itsType;
    /**
     * @brief itsSprite the name of the file containing the pokemon's sprite
     */
    std::string itsSprite;
    /**
     * @brief itsHP the pokemon's current hp
     */
    int itsHP;
    /**
     * @brief itsMaxHp the pokemon's max hp
     */
    int itsMaxHp;
    /**
     * @brief itsAtk the pokemon's attack stat
     */
    int itsAtk;
    /**
     * @brief itsDef the pokemon's defense stat
     */
    int itsDef;
    /**
     * @brief itsSpeed the pokemon's speed stat
     */
    float itsSpeed;
    /**
     * @brief itsSize the pokemon's size in m
     */
    float itsSize;
    /**
     * @brief itsWeight the pokemon's weight in kg
     */
    float itsWeight;
    /**
     * @brief itsMaxHP the pokemon's maximum hp
     */
    int itsMaxHP;

public:
    /**
     * @brief Pokemon the constructor of the Pokemon class
     * @param number the pokemon's pokedex number
     * @param name the pokemon's name
     * @param sprite the name of the file that contains the pokemon's sprite
     * @param hp the pokemon's hp
     * @param atk the pokemon's attack stat
     * @param def the pokemon's defense stat
     * @param size the pokemon's size in meters
     * @param weight the pokemon's weight in kilograms
     * @param type the pokemon's type
     */
    Pokemon(int number, std::string name, std::string sprite, int hp, int atk, int def, int speed, std::string type);
    ~Pokemon();
    //getters
    /**
     * @brief getItsName returns the pokemon's name
     * @return the pokemon's name
     */
    std::string getItsName();
    /**
     * @brief getItsSprite returns the name of the file containing the pokemon's sprite
     * @return the name of the file containing the pokemon's sprite
     */
    std::string getItsSprite();
    /**
     * @brief getItsNumber returns the pokemon's pokedex number
     * @return the pokemon's pokedex number
     */
    int getItsNumber();
    /**
     * @brief getItsHp returns the pokemon's current hp
     * @return the pokemon's current hp
     */
    int getItsHp();
    /**
     * @brief getItsMaxHp returns the pokemon's maximum hp count
     * @return the pokemon's maximum hp count
     */
    int getItsMaxHp();
    /**
     * @brief getItsAtk returns the pokemon's attack stat
     * @return the pokemon's attack stat
     */
    int getItsAtk();
    /**
     * @brief getItsDef returns the pokemon's defense stat
     * @return the pokemon's defense stat
     */
    int getItsDef();
    /**
     * @brief getItsSpeed returns the pokemon's speed stat
     * @return the pokemon's speed stat
     */
    int getItsSpeed();
    /**
     * @brief getItsType returns the pokemon's type
     * @return the pokemon's type
     */
    std::string getItsType();

    //setters
    /**
     * @brief heal set the pokemon's hp to its max hp, effectively healing it completely
     */
    void heal();

    /**
     * @brief takeDamage reduces the pokemon's hp by the damage given as argument
     * @param dmg the damage count to be deducted from the pokemon's hp
     */
    void takeDamage(int dmg);
    /**
     * @brief typeEfficacity returns the efficiency multiplier for typeAtk attacking typeDef
     * @param typeAtk the type of the attacking pokemon
     * @param typeDef the type of the defending pokemon
     * @return the type efficiency multiplier for typeAtk attacking typeDef
     */
    float typeEfficacity(std::string typeAtk, std::string typeDef);
    /**
     * @brief howMuchFaster returns the number of attacks a pokemon can launch before his slower opponent can attack
     * @param slowest a pokemon, that has been found to be slower than his opponent
     * @return the faster pokemon's speed divided by the slower pokemon's speed, effectively giving the number of attacks a pokemon can launch before his slower opponent can attack
     */
    int howMuchFaster(Pokemon* slowest);
};

#endif // POKEMON_H

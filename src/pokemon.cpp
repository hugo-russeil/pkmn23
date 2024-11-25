#include "pokemon.h"
#include "typeTable.h"
#include <iostream>
#include <random>

Pokemon::Pokemon(int number, std::string name, std::string sprite, int hp, int atk, int def, int speed, std::string type)
{
    itsNumber = number;
    itsName = name;
    itsHP = hp;
    itsMaxHP = hp;
    itsAtk = atk;
    itsDef = def;
    itsSpeed = speed;
    itsSprite = sprite;
    itsType = type;
    
}

Pokemon::~Pokemon(){

}

std::string Pokemon::getItsName()
{
    return itsName;
}

std::string Pokemon::getItsSprite()
{
    return itsSprite;
}

int Pokemon::getItsNumber()
{
    return itsNumber;
}

void Pokemon::takeDamage(int dmg)
{
    if(dmg > itsHP) dmg = itsHP;
    itsHP -= dmg;
}

std::string Pokemon::getItsType()
{
    return itsType;
}

void Pokemon::heal()
{
    itsHP = itsMaxHP;
}

int Pokemon::getItsHp(){
    return itsHP;
}

int Pokemon::getItsMaxHp()
{
    return itsMaxHP;
}

int Pokemon::getItsAtk()
{
    return itsAtk;
}

int Pokemon::getItsDef()
{
    return itsDef;
}

int Pokemon::getItsSpeed()
{
    return itsSpeed;
}

float Pokemon::typeEfficacity(std::string typeAtk, std::string typeDef)
{
    float efficiency = typesChart[typesMap[typeAtk]][typesMap[typeDef]];
    return efficiency;
}

int Pokemon::howMuchFaster(Pokemon *slowest)
{
    return getItsSpeed()/slowest->getItsSpeed();
}

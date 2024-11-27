#include "pokemon.h"
#include "typeTable.h"
#include <iostream>
#include <random>

Pokemon::Pokemon(int number, std::string name, std::string sprite, int hp, int atk, int def, int special, int speed, std::string type)
{
    itsNumber = number;
    itsName = name;
    itsHP = hp;
    itsMaxHP = hp;
    itsAtk = atk;
    itsDef = def;
    itsSpecial = special;
    itsSpeed = speed;
    itsSprite = sprite;
    itsType = type;
    itsMoves = std::vector<Move>();
    
    // All moves will be hardcoded for now
    Move firstMove = moveFromJson("Thunder");
    Move secondMove = moveFromJson("Tackle");
    Move thirdMove = moveFromJson("Scratch");
    Move fourthMove = moveFromJson("Teleport"); // Attack that will always miss to test the accuracy system

    itsMoves.push_back(firstMove);
    itsMoves.push_back(secondMove);
    itsMoves.push_back(thirdMove);
    itsMoves.push_back(fourthMove);
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

int Pokemon::getItsSpecial()
{
    return itsSpecial;
}

int Pokemon::getItsSpeed()
{
    return itsSpeed;
}

std::vector<Move> Pokemon::getItsMoves(){
    return itsMoves;
}

float computeTypeEfficacity(std::string typeAtk, std::string typeDef)
{
    float efficiency = typesChart[typesMap[typeAtk]][typesMap[typeDef]];
    std::cout << "Type efficacity of " << typeAtk << " against " << typeDef << " is " << efficiency << std::endl;
    return efficiency;
}

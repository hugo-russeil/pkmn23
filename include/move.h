#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "jsonHandler.h"

/**
 * @struct StatsModifier
 * @brief Represents a modifier to a Pokémon's stats.
 */
struct StatsModifier {
    /**
     * @enum Stat
     * @brief The type of stat being modified.
     */
    enum class Stat { Attack, Defense, Speed, Special, None };

    /**
     * @enum Modifier
     * @brief The type of modification being applied.
     */
    enum class Modifier { Increase, Decrease, None };

    int amount; /**< The amount of the modification. */
    Stat stat; /**< The type of stat being modified. */
    Modifier modifier; /**< The type of modification being applied. */
};

/**
 * @class Move
 * @brief Represents a Pokémon move.
 */
class Move {
public:
    /**
     * @enum Category
     * @brief The category of the move.
     */
    enum class Category { Physical, Special, Status };

    /**
     * @enum StatusModifier
     * @brief The status effect caused by the move.
     */
    enum class StatusModifier { None, Burn, Paralysis, Freeze, Poison, Sleep };

private:
    std::string name; /**< The name of the move. */
    int basePower; /**< The base power of the move. */
    int accuracy; /**< The accuracy of the move (0-100). */
    std::string type; /**< The type of the move. */
    Category category; /**< The category of the move. */
    int pp; /**< The remaining uses of the move. */
    int maxPp; /**< The maximum uses of the move. */
    StatusModifier statusModifier; /**< The status modifier of the move. */
    struct StatsModifier statsModifier; /**< The stats modifier of the move. */
    int priority; /**< The priority of the move. */
    int critChance; /**< The critical hit chance of the move. */
    std::string specialMechanics; /**< The special mechanics of the move. */

public:
    /**
     * @brief Constructs a new Move object.
     * @param name The name of the move.
     * @param basePower The base power of the move.
     * @param accuracy The accuracy of the move (0-100).
     * @param type The type of the move.
     * @param category The category of the move.
     * @param pp The remaining uses of the move.
     * @param statusModifier The status modifier of the move.
     * @param statsModifier The stats modifier of the move.
     * @param priority The priority of the move.
     * @param critChance The critical hit chance of the move.
     * @param specialMechanics The special mechanics of the move.
     */
    Move(
        std::string name, 
        int basePower, 
        int accuracy, 
        std::string type, 
        Category category,  // Removed "enum class"
        int pp, 
        StatusModifier statusModifier,  // Removed "enum class"
        StatsModifier statsModifier, 
        int priority, 
        int critChance, 
        std::string specialMechanics
    );

    /**
     * @brief Destructs the Move object.
     */
    ~Move();

    Move moveFromJson(std::string moveName);

    // Getters
    std::string getName() const;
    int getBasePower() const;
    int getAccuracy() const;
    std::string getType() const;
    Category getCategory() const;
    int getPp() const;
    StatusModifier getStatusModifier() const;
    StatsModifier getStatsModifier() const;
    int getPriority() const;
    int getCritChance() const;
    std::string getSpecialMechanics() const;

    // Setter
    void setPp(int newPP);
};

#endif // MOVE_H

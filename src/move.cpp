#include "move.h"
#include "typeTable.h"  // Include the type validation header
#include <stdexcept>
#include <iostream>

// Constructor with type validation
Move::Move(
    std::string name, 
    int basePower, 
    int accuracy, 
    std::string type, 
    Category category,  // Fixed parameter
    int pp, 
    StatusModifier statusModifier,  // Fixed parameter
    StatsModifier statsModifier, 
    int priority, 
    int critChance, 
    std::string specialMechanics
) 
    : name(std::move(name)), 
      basePower(basePower), 
      accuracy(accuracy), 
      type(std::move(type)), 
      category(category),  // Initialised correctly
      pp(pp), 
      maxPp(pp), 
      statusModifier(statusModifier),  // Initialised correctly
      statsModifier(statsModifier), 
      priority(priority), 
      critChance(critChance), 
      specialMechanics(std::move(specialMechanics)) 
{
    // Type validation using the typesMap from TypeTable.h
    if (typesMap.find(type) == typesMap.end()) {
        throw std::invalid_argument("Invalid type specified: " + type);
    }

    this->type = type;

    // Validity check for accuracy
    if (accuracy < 0 || accuracy > 100) {
        throw std::out_of_range("Accuracy must be between 0 and 100.");
    }
    
    // Validity check for pp
    if (pp <= 0) {
        throw std::invalid_argument("PP must be a positive number.");
    }
    
    // Validity check for critChance (assuming percentage)
    if (critChance < 0 || critChance > 100) {
        throw std::out_of_range("Critical chance must be between 0 and 100.");
    }
}

// Destructor
Move::~Move() = default;

Move Move::moveFromJson(std::string moveName) {
    // Open the JSON file
    std::ifstream file("moves.json");
    if (!file) {
        throw std::runtime_error("Failed to open the moves file.");
    }

    // Read the entire content of the file into a string
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Parse the JSON content
    jsonHandler json = jsonHandler::parse(jsonContent);

    // Check if the move exists in the JSON object
    if (json.object.find(moveName) == json.object.end()) {
        throw std::invalid_argument("Move not found: " + moveName);
    }

    // Extract the move data from the JSON object
    const jsonHandler& moveData = json.object.at(moveName);

    // Parse the attributes from the JSON data
    std::string name = moveName;  // Move name passed as argument
    int basePower = moveData.object.at("basePower").numberValue;
    int accuracy = moveData.object.at("accuracy").numberValue;
    std::string type = moveData.object.at("type").stringValue;
    Category category = (moveData.object.at("category").stringValue == "Physical") ? Category::Physical :
                        (moveData.object.at("category").stringValue == "Special") ? Category::Special : Category::Status;
    int pp = moveData.object.at("pp").numberValue;
    
    // Status modifier (assuming it's a string like "None", "Burn", etc.)
    StatusModifier statusModifier = (moveData.object.at("statusModifier").stringValue == "None") ? StatusModifier::None :
                                     (moveData.object.at("statusModifier").stringValue == "Burn") ? StatusModifier::Burn :
                                     (moveData.object.at("statusModifier").stringValue == "Paralysis") ? StatusModifier::Paralysis :
                                     (moveData.object.at("statusModifier").stringValue == "Freeze") ? StatusModifier::Freeze :
                                     (moveData.object.at("statusModifier").stringValue == "Poison") ? StatusModifier::Poison : StatusModifier::Sleep;

    // Stats modifier
    StatsModifier statsModifier;
    statsModifier.amount = moveData.object.at("statsModifier").object.at("amount").numberValue;
    statsModifier.stat = (moveData.object.at("statsModifier").object.at("stat").stringValue == "Attack") ? StatsModifier::Stat::Attack :
                         (moveData.object.at("statsModifier").object.at("stat").stringValue == "Defense") ? StatsModifier::Stat::Defense :
                         (moveData.object.at("statsModifier").object.at("stat").stringValue == "Speed") ? StatsModifier::Stat::Speed :
                         (moveData.object.at("statsModifier").object.at("stat").stringValue == "Special") ? StatsModifier::Stat::Special : StatsModifier::Stat::None;
    statsModifier.modifier = (moveData.object.at("statsModifier").object.at("modifier").stringValue == "Increase") ? StatsModifier::Modifier::Increase :
                             (moveData.object.at("statsModifier").object.at("modifier").stringValue == "Decrease") ? StatsModifier::Modifier::Decrease : StatsModifier::Modifier::None;

    int priority = moveData.object.at("priority").numberValue;
    int critChance = moveData.object.at("critChance").numberValue;
    std::string specialMechanics = moveData.object.at("specialMechanics").stringValue;

    // Construct and return the Move object
    return Move(name, basePower, accuracy, type, category, pp, statusModifier, statsModifier, priority, critChance, specialMechanics);
}


// Getters
std::string Move::getName() const {
    return name;
}

int Move::getBasePower() const {
    return basePower;
}

int Move::getAccuracy() const {
    return accuracy;
}

std::string Move::getType() const {
    return type;
}

Move::Category Move::getCategory() const {
    return category;
}

int Move::getPp() const {
    return pp;
}

Move::StatusModifier Move::getStatusModifier() const {
    return statusModifier;
}

StatsModifier Move::getStatsModifier() const {
    return statsModifier;
}

int Move::getPriority() const {
    return priority;
}

int Move::getCritChance() const {
    return critChance;
}

std::string Move::getSpecialMechanics() const {
    return specialMechanics;
}

// Setter
void Move::setPp(int newPP) {
    if (newPP >= 0 && newPP <= maxPp) {
        pp = newPP;
    } else {
        throw std::out_of_range("PP must be between 0 and the maximum PP.");
    }
}

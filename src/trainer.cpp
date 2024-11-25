#include <termios.h>
#include <unistd.h>
#define CLEAR system("clear")
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_LEFT 68
#define KEY_RIGHT 67
#define ENTER 10

#include "trainer.h"
#include "jsonHandler.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <random>

Trainer::Trainer(std::string name)
{
    itsName = name;
    itsXp = 0;
    itsLvl = 1;
    itsTeam = std::vector<Pokemon*>();
    isHuman = false;
}

std::string Trainer::getItsName()
{
    return itsName;
}

void Trainer::setIsHuman(bool isHuman)
{
    this->isHuman = isHuman;
}

bool Trainer::getIsHuman()
{
    return isHuman;
}

int Trainer::getItsLevel()
{
    return itsLvl;
}

int Trainer::getItsXp()
{
    return itsXp;
}

std::vector<Pokemon *> Trainer::getItsTeam()
{
    return itsTeam;
}

void Trainer::catchPokemon(int nb) {
    // Step 1: Read the JSON file
    std::ifstream file("pokemons.json");
    if (!file) {
        std::cout << "Failed to open the pokemons JSON file." << std::endl;
        return;
    }

    // Read the entire file into a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Step 2: Parse the JSON content
    jsonHandler json = jsonHandler::parse(buffer.str());
    if (json.type != jsonHandler::Type::ARRAY) {
        std::cout << "Invalid JSON format: Expected an array of Pokémon." << std::endl;
        return;
    }

    // Step 3: Access the Pokémon at index `nb`
    if (nb < 0 || nb >= json.array.size()) {
        std::cout << "Invalid Pokémon index: " << nb << std::endl;
        return;
    }

    jsonHandler pokemonData = json.array[nb];
    if (pokemonData.type != jsonHandler::Type::OBJECT) {
        std::cout << "Invalid data for Pokémon at index " << nb << std::endl;
        return;
    }

    // Step 4: Extract Pokémon details
    int id = pokemonData.object["id"].numberValue;
    std::string name = pokemonData.object["name"].stringValue;
    std::string type = pokemonData.object["type"].stringValue;
    std::string sprite = pokemonData.object["sprite"].stringValue;

    jsonHandler stats = pokemonData.object["stats"];
    if (stats.type != jsonHandler::Type::OBJECT) {
        std::cout << "Invalid stats data for Pokémon." << std::endl;
        return;
    }

    int hp = stats.object["hp"].numberValue;
    int atk = stats.object["attack"].numberValue;
    int def = stats.object["defense"].numberValue;
    float speed = stats.object["speed"].numberValue;

    // Step 5: Create the Pokémon object and add it to the team
    Pokemon* pokemon = new Pokemon(id, name, sprite, hp, atk, def, speed, type);
    Trainer::itsTeam.push_back(pokemon);

    // Step 6: Gain XP
    Trainer::gainXp(1);
}

int Trainer::getItsAvgTeamSpeed()
{
    int avg = 0;
    for (Pokemon* pokemon : itsTeam) {
        avg += pokemon->getItsSpeed();
    }
    avg = avg/6;
    return avg;
}

void Trainer::displayAvgTeamSpeed()
{
    std::cout << "the average speed of  " << itsName << "'s team is " << getItsAvgTeamSpeed() << std::endl;
    std::cout << std::endl;
    while (std::cin.get()!='\n');
    CLEAR;
}

void Trainer::displayAvgTeamSpeedForType(std::string type)
{
    int avg = 0;
    int count = 0;
    for (Pokemon* pokemon : itsTeam) {
        if(pokemon->getItsType() == type){
            avg += pokemon->getItsSpeed();
            count++;
        }
    }
    if(count != 0){
        avg = avg/count;
        std::cout << "The average speed of  " << itsName << "'s team for the " << type << " type is " << avg << std::endl;
        std::cout << std::endl;
    } else std::cout << itsName << " has no pokemon of "  << type << " type" << std::endl << std::endl;
    while (std::cin.get()!='\n');
    CLEAR;

}

bool Trainer::transferPokemon(){
    if(itsXp >= 2){
        int index = this->teamMenu();
        std::cout << itsTeam.at(index)->getItsName() << " has been transfered !" << std::endl;
        delete itsTeam.at(index);
        itsTeam.erase(itsTeam.begin() + index);
        looseXp(1); //loose three but regains one with catchPokemon()
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> distr(0, 151);
        catchPokemon(distr(eng));
        std::cout << "It was replaced with a " << itsTeam.at(5)->getItsName() << std::endl;
        std::cout << std::endl;
        while (std::cin.get()!='\n');
        CLEAR;
        return true;
    }
    else{
        std::cout << "Not enough xp to transfer a pokemon" << std::endl;
        std::cout << std::endl;
        while (std::cin.get()!='\n');
        CLEAR;
        return false;
    }
}

void Trainer::gainXp(int xp)
{
    itsXp += xp;
    if(itsXp > 10){
        itsXp -= 10;
        itsLvl++;
    }
}

void Trainer::setLevel(int lvl)
{
    itsLvl = lvl;
}

void Trainer::looseXp(int xp)
{
    itsXp -= xp;
    if(itsXp < 0) itsXp = 0;
}

bool Trainer::hasTrainerLost()
{
    for (Pokemon* pokemon : itsTeam) {
        if(pokemon->getItsHp() > 0){
            return false;
        }
    }
    return true;
}

void Trainer::healTeam()
{
    if(this->getIsHuman() == true){
        for (Pokemon* pokemon : itsTeam) {
            if(pokemon->getItsHp() > 0){
                pokemon->heal();
            }
        }
    }
    else{
        for (int index = 0; index < 6; index++) {
            delete itsTeam.at(0);
            itsTeam.erase(itsTeam.begin());
            std::random_device rd;
            std::default_random_engine eng(rd());
            std::uniform_int_distribution<int> distr(0, 89);
            catchPokemon(distr(eng));
        }
    }
}

void Trainer::saveTrainer() {
    // Step 1: Read the existing JSON save file
    std::ifstream saveFile("saveGames.json");
    jsonHandler jsonData(jsonHandler::Type::ARRAY); // Assume an array of trainers

    if (saveFile.is_open()) {
        // Read the entire file into a string
        std::stringstream buffer;
        buffer << saveFile.rdbuf();
        saveFile.close();

        // Parse the JSON content
        std::string jsonString = buffer.str();
        if (!jsonString.empty()) {
            jsonData = jsonHandler::parse(jsonString);
        } else {
            jsonData = jsonHandler(jsonHandler::Type::ARRAY); // Initialise as an empty array
        }
    } else {
        // If the file doesn't exist, initialise jsonData as an empty array
        jsonData = jsonHandler(jsonHandler::Type::ARRAY);
    }

    // Step 2: Check if the JSON data is indeed an array
    if (jsonData.type != jsonHandler::Type::ARRAY) {
        std::cerr << "Invalid JSON format: Expected an array for save data." << std::endl;
        return;
    }

    // Step 3: Update or add the trainer's profile in the JSON array
    bool trainerFound = false;
    for (auto& trainer : jsonData.array) {
        if (trainer.type == jsonHandler::Type::OBJECT && 
            trainer.object["name"].stringValue == itsName) {
            // Update existing trainer's data with correctly ordered keys
            trainer.object["name"] = jsonHandler(itsName);  // Ensure 'name' is the first key
            trainer.object["level"] = jsonHandler(static_cast<int>(itsLvl)); // Store level as an integer
            trainer.object["xp"] = jsonHandler(static_cast<int>(itsXp));     // Store XP as an integer

            // Update Pokémon list
            jsonHandler pokemonArray(jsonHandler::Type::ARRAY);
            for (auto& pokemon : itsTeam) {
                pokemonArray.array.push_back(jsonHandler(static_cast<int>(pokemon->getItsNumber())));
            }
            trainer.object["pokemon"] = pokemonArray;

            trainerFound = true;
            break;
        }
    }

    // If trainer not found, create a new entry
    if (!trainerFound) {
        jsonHandler newTrainer(jsonHandler::Type::OBJECT);

        // Explicitly set keys in the desired order
        newTrainer.object["name"] = jsonHandler(itsName);
        newTrainer.object["level"] = jsonHandler(static_cast<int>(itsLvl)); // Store level as an integer
        newTrainer.object["xp"] = jsonHandler(static_cast<int>(itsXp));     // Store XP as an integer

        // Create the Pokémon array
        jsonHandler pokemonArray(jsonHandler::Type::ARRAY);
        for (auto& pokemon : itsTeam) {
            pokemonArray.array.push_back(jsonHandler(static_cast<int>(pokemon->getItsNumber())));
        }
        newTrainer.object["pokemon"] = pokemonArray;

        // Add the new trainer to the JSON data array
        jsonData.array.push_back(newTrainer);
    }

    // Step 4: Write the updated JSON data back to the file
    std::ofstream outputFile("saveGames.json", std::ios::trunc);  // Truncate mode for writing
    if (outputFile.is_open()) {
        // Convert jsonData back to a string
        outputFile << jsonData.toString(); // Assuming `toString` formats the JSON with correct key order
        outputFile.close();
        std::cout << itsName << "'s profile saved successfully." << std::endl;
    } else {
        std::cerr << "Unable to open the file for saving the trainer profile." << std::endl;
    }
}

Trainer* Trainer::loadTrainer(std::string playerName) {
    // Step 1: Read the existing JSON save file
    std::ifstream saveFile("saveGames.json");
    if (!saveFile) {
        std::cout << "Unable to open the save game file.\n";
        // Create a new trainer with default values if the file can't be opened
        Trainer* trainer = new Trainer(playerName);
        trainer->setLevel(1);
        trainer->gainXp(0);
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> distr(1, 151); // Ensuring valid Pokémon range
        for (int i = 6; i > 0; i++) {
            trainer->catchPokemon(distr(eng));
        }
        return trainer;
    }

    // Read the entire file into a string
    std::stringstream buffer;
    buffer << saveFile.rdbuf();
    saveFile.close();

    // Parse the JSON content
    jsonHandler jsonData;
    std::string jsonString = buffer.str();
    if (!jsonString.empty()) {
        jsonData = jsonHandler::parse(jsonString);
    } else {
        std::cerr << "Empty save game file.\n";
        return nullptr;
    }

    // Check if the JSON data is an array
    if (jsonData.type != jsonHandler::Type::ARRAY) {
        std::cerr << "Invalid JSON format: Expected an array of trainers.\n";
        return nullptr;
    }

    // Step 2: Search for the trainer with the given playerName
    for (auto& trainer : jsonData.array) {
        if (trainer.type == jsonHandler::Type::OBJECT && trainer.object["name"].stringValue == playerName) {
            // Trainer found, extract the data
            std::string name = trainer.object["name"].stringValue;
            int level = static_cast<int>(trainer.object["level"].numberValue);
            int xp = static_cast<int>(trainer.object["xp"].numberValue);

            Trainer* loadedTrainer = new Trainer(name);
            loadedTrainer->setLevel(level);
            loadedTrainer->gainXp(xp);

            // Extract Pokémon numbers
            if (trainer.object["pokemon"].type == jsonHandler::Type::ARRAY) {
                for (auto& pokemonNumber : trainer.object["pokemon"].array) {
                    int number = 0; // Default to 0 (missingNo) in case of invalid data
                    if (pokemonNumber.type == jsonHandler::Type::NUMBER) {
                        int potentialNumber = static_cast<int>(pokemonNumber.numberValue);
                        if (potentialNumber >= 1 && potentialNumber <= 151) {
                            number = potentialNumber;
                        }
                    }
                    // If invalid or out of range, number will remain 0 (missingNo)
                    loadedTrainer->catchPokemon(number);
                }
            }

            std::cout << "Trainer profile loaded successfully." << std::endl;
            return loadedTrainer;
        }
    }

    // Step 3: If the trainer is not found, create a new trainer with default values
    std::cout << "Trainer profile not found." << std::endl;
    Trainer* newTrainer = new Trainer(playerName);
    newTrainer->setLevel(1);
    newTrainer->gainXp(0);

    // Generate 6 random Pokémon for the new trainer
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(1, 151); // Ensuring valid Pokémon range
    for (int i = 6; i > 0; i++) {
        newTrainer->catchPokemon(distr(eng));
    }

    return newTrainer;
}




void Trainer::choosePokemon()
{
    if(!this->hasTrainerLost()){
        CLEAR;
        int index = 0;
        std::cout << getItsName() << ", which pokemon will you send to battle ? \n" << std::endl;
        while (std::cin.get()!='\n');
        CLEAR;
        index = this->teamMenu();
        if(getItsTeam().at(index)->getItsHp() > 0) sendToBattle(index);
        else{
            std::cout << getItsTeam().at(index)->getItsName() << " is in no condition to fight !";
            while (std::cin.get()!='\n');
            this->choosePokemon();
        }
    } else return;
}

void Trainer::botChoosePokemon(Pokemon *opponentsPokemon, int behaviour)
{
    if(!this->hasTrainerLost()){

        if(behaviour == 0){
            int index = 0;
            //Sends whatever pokemon it finds in the bot's team.
            for (Pokemon* pokemon : getItsTeam()){
                if(pokemon->getItsHp() > 0){
                    sendToBattle(index);
                    return;
                }
                index++;
            }
        }
        else{
            int index = 0;
            //Searches for a pokemon with a type advantage over the opponent
            for (Pokemon* pokemon : getItsTeam()){
                if(pokemon->typeEfficacity(pokemon->getItsType(), opponentsPokemon->getItsType()) == 2 && pokemon->getItsHp() > 0){
                    sendToBattle(index);
                    return;
                }
                index++;
            }
            //If it doesn't find one, searches for a pokemon with no type weakness towards the opponent
            index = 0;
            for (Pokemon* pokemon : getItsTeam()){
                if(pokemon->typeEfficacity(pokemon->getItsType(), opponentsPokemon->getItsType()) == 1 && pokemon->getItsHp() > 0){
                    sendToBattle(index);
                    return;
                }
                index++;
            }
            index = 0;
             //If it doesn't find one, searches for a pokemon...
            for (Pokemon* pokemon : getItsTeam()){
                if(pokemon->getItsHp() > 0){
                    sendToBattle(index);
                    return;
                }
                index++;
            }
        }
    } else return;
}

void Trainer::sendToBattle(int teamRank)
{
    std::iter_swap(itsTeam.begin(), itsTeam.begin() + teamRank);
    std::cout << itsTeam.at(0)->getItsName() << ", I choose you !" << std::endl;
    while (std::cin.get()!='\n');
    #if defined __unix__ || __APPLE__
    CLEAR;
    #endif
}

#if defined __unix__ || __APPLE__
static int getch()
{
    struct termios oldSettings, newSettings;
    tcgetattr(STDIN_FILENO, &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    int ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    return ch;
}
#endif

int Trainer::teamMenu(int selected)
{
    int index = 0;
    for (Pokemon* pokemon : itsTeam) {
        if(pokemon->getItsHp() < 1){
            #if defined _WIN32
            std::cout << "\x1b[0m" << "\x1b[1;31m" << "\x1b[47m"; //reset colours, red foreground, white background
            #elif defined __unix__ || __APPLE__
            std::cout << "\x1b[1;31m"; //red foreground
            #endif
        }
        if(index == selected){
            std::cout << ">" << pokemon->getItsName() << "  ";
        }
        else{
            std::cout << " " << pokemon->getItsName() << "  ";
        }
        index++;
        if(index == 3) std::cout << std::endl;
        #if defined _WIN32
        std::cout << "\x1b[0m" << "\x1b[30m" << "\x1b[47m"; //reset colours, black foreground, white background
        #elif defined __unix__ || __APPLE__
        std::cout << "\x1b[0m"; //reset colours
        #endif
    }
    std::cout << std::endl;
    while(1)
    {
        switch((getch())) {
        case KEY_UP:
            if(selected > 2) selected-=3;
            break;
        case KEY_DOWN:
            if(selected < 3) selected+=3;
            break;
        case KEY_LEFT:
            if(selected > 0) selected--;
            break;
        case KEY_RIGHT:
            if(selected < 5) selected++;
            break;
        case ENTER:
            CLEAR;
            return selected;
        default:
            break;
        }
        CLEAR;
        int result = teamMenu(selected);
        return result;
    }
}




int Trainer::interBattleMenu(int selected)
{
    while (true) {
        CLEAR;
        std::cout << "Player " << this->getItsName() << std::endl;
        if (selected == 0)
            std::cout << "> To battle !  ";
        else
            std::cout << "  To battle !  ";

        if (selected == 1)
            std::cout << "> View team     ";
        else
            std::cout << "  View team     ";

        if (selected == 2)
            std::cout << "> Transfer pokemon  ";
        else
            std::cout << "  Transfer pokemon  ";

        std::cout << std::endl;

        switch (getch()) {
        case KEY_LEFT:
            if (selected > 0)
                selected--;
            break;
        case KEY_RIGHT:
            if (selected < 2)
                selected++;
            break;
        case ENTER:
            switch (selected) {
            case 0:
                CLEAR;
                return selected;
            case 1:
                CLEAR;
                teamMenu(0); // Display team menu and wait for enter
                CLEAR;
                break;
            case 2:
                CLEAR;
                transferPokemon();
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}


void Trainer::compareTrainers(Trainer *defender)
{
    int scoreTrainer1 = 0;
    int scoreTrainer2 = 0;

    int totalHpTrainer1 = 0;
    int totalAtkTrainer1 = 0;

    for (Pokemon* pokemon : itsTeam) {
        totalHpTrainer1 += pokemon->getItsHp();
        totalAtkTrainer1 += pokemon->getItsAtk();
    }

    int totalHpTrainer2 = 0;
    int totalAtkTrainer2 = 0;

    for (Pokemon* pokemon : defender->itsTeam) {
        totalHpTrainer2 += pokemon->getItsHp();
        totalAtkTrainer2 += pokemon->getItsAtk();
    }

    if(getItsAvgTeamSpeed() > defender->getItsAvgTeamSpeed()) scoreTrainer1++;
    else if(getItsAvgTeamSpeed() < defender->getItsAvgTeamSpeed()) scoreTrainer2++;

    if(itsLvl > defender->itsLvl) scoreTrainer1++;
    else if(itsLvl < defender->itsLvl) scoreTrainer2++;

    if(totalAtkTrainer1 > totalAtkTrainer2) scoreTrainer1++;
    else if(totalAtkTrainer1 < totalAtkTrainer2) scoreTrainer2++;

    if(totalHpTrainer1 > totalHpTrainer2) scoreTrainer1++;
    else if(totalHpTrainer1 < totalHpTrainer2) scoreTrainer2++;

    if(itsLvl == defender->itsLvl){
        if(itsXp > defender->itsXp) scoreTrainer1++;
        else if(itsXp < defender->itsXp) scoreTrainer2++;
    }
    for(int i = 11 - this->getItsName().length(); i > 0; i--){
        std::cout << " ";
    }
    std::cout << this->getItsName() << " - " << defender->getItsName() << std::endl;
    std::cout << " HP:    " << totalHpTrainer1 << " - " << totalHpTrainer2 << std::endl;
    std::cout << "Atk:    " << totalAtkTrainer1 << " - " << totalAtkTrainer2 << std::endl;
    std::cout << "Spd:      " << getItsAvgTeamSpeed() << " - " << defender->getItsAvgTeamSpeed() << std::endl;
    std::cout << "lvl:  " << itsLvl << "+" << itsXp << "xp" << " - " << defender->itsLvl << "+" << defender->itsXp << "xp" <<std::endl;
    std::cout << std::endl;

    while (std::cin.get()!='\n');

    if(scoreTrainer1 > scoreTrainer2) std::cout << itsName << " is the favourite for this fight !" << std::endl;
    else if(scoreTrainer1 < scoreTrainer2) std::cout << defender->itsName << " is the favourite for this fight !" << std::endl;
    else std::cout << "The results of this fight are most uncertain !" << std::endl;
    std::cout << std::endl;
    while (std::cin.get()!='\n');
    CLEAR;
}



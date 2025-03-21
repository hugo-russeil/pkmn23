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

std::vector<Pokemon *> Trainer::getItsTeam()
{
    return itsTeam;
}

void Trainer::catchPokemon(int nb) {
    // Read the JSON file
    std::ifstream file("pokemons.json");
    if (!file) {
        std::cout << "Failed to open the pokemons JSON file." << std::endl;
        return;
    }

    // Read the entire file into a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Parse the JSON content
    jsonHandler json = jsonHandler::parse(buffer.str());
    if (json.type != jsonHandler::Type::ARRAY) {
        std::cout << "Invalid JSON format: Expected an array of Pokémon." << std::endl;
        return;
    }

    // Access the Pokémon at index `nb`
    if (nb < 0 || nb >= json.array.size()) {
        std::cout << "Invalid Pokémon index: " << nb << std::endl;
        return;
    }

    jsonHandler pokemonData = json.array[nb];
    if (pokemonData.type != jsonHandler::Type::OBJECT) {
        std::cout << "Invalid data for Pokémon at index " << nb << std::endl;
        return;
    }

    // Extract Pokémon details
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
    int special = stats.object["special"].numberValue;
    float speed = stats.object["speed"].numberValue;

    // Create the Pokémon object and add it to the team
    Pokemon* pokemon = new Pokemon(id, name, sprite, hp, atk, def, special, speed, type);
    Trainer::itsTeam.push_back(pokemon);

}

bool Trainer::transferPokemon(){
    int index = this->teamMenu();
    std::cout << itsTeam.at(index)->getItsName() << " has been transfered !" << std::endl;
    delete itsTeam.at(index);
    itsTeam.erase(itsTeam.begin() + index);
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
            pokemon->heal();
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
    // Read the existing JSON save file
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

    // Check if the JSON data is indeed an array
    if (jsonData.type != jsonHandler::Type::ARRAY) {
        std::cerr << "Invalid JSON format: Expected an array for save data." << std::endl;
        return;
    }

    // Update or add the trainer's profile in the JSON array
    bool trainerFound = false;
    for (auto& trainer : jsonData.array) {
        if (trainer.type == jsonHandler::Type::OBJECT && 
            trainer.object["name"].stringValue == itsName) {
            // Update existing trainer's data with correctly ordered keys
            trainer.object["name"] = jsonHandler(itsName);  // Ensure 'name' is the first key

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

        // Create the Pokémon array
        jsonHandler pokemonArray(jsonHandler::Type::ARRAY);
        for (auto& pokemon : itsTeam) {
            pokemonArray.array.push_back(jsonHandler(static_cast<int>(pokemon->getItsNumber())));
        }
        newTrainer.object["pokemon"] = pokemonArray;

        // Add the new trainer to the JSON data array
        jsonData.array.push_back(newTrainer);
    }

    // Write the updated JSON data back to the file
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
    // Read the existing JSON save file
    std::ifstream saveFile("saveGames.json");
    if (!saveFile) {
        std::cout << "Unable to open the save game file.\n";
        // Create a new trainer with default values if the file can't be opened
        Trainer* trainer = new Trainer(playerName);
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

    // Search for the trainer with the given playerName
    for (auto& trainer : jsonData.array) {
        if (trainer.type == jsonHandler::Type::OBJECT && trainer.object["name"].stringValue == playerName) {
            // Trainer found, extract the data
            std::string name = trainer.object["name"].stringValue;

            Trainer* loadedTrainer = new Trainer(name);

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

    // If the trainer is not found, create a new trainer with default values
    std::cout << "Trainer profile not found." << std::endl;
    Trainer* newTrainer = new Trainer(playerName);

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

void Trainer::botChoosePokemon(Pokemon *opponentsPokemon)
{
    if(!this->hasTrainerLost()){
        
        int index = 0;
        //Searches for a pokemon with a type advantage over the opponent
        for (Pokemon* pokemon : getItsTeam()){
            if(computeTypeEfficacity(pokemon->getItsType(), opponentsPokemon->getItsType()) == 2 && pokemon->getItsHp() > 0){
                sendToBattle(index);
                return;
            }
            index++;
        }
        //If it doesn't find one, searches for a pokemon with no type weakness towards the opponent
        index = 0;
        for (Pokemon* pokemon : getItsTeam()){
            if(computeTypeEfficacity(pokemon->getItsType(), opponentsPokemon->getItsType()) == 1 && pokemon->getItsHp() > 0){
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

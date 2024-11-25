#include <termios.h>
#include <unistd.h>
#include <curses.h>
#define CLEAR system("clear")
#define ENTER 10

#include "game.h"
#include <random>
#include <fstream>
#include <chrono>
#include <thread>

Game::Game()
{

}

void Game::gameLoop()
{
    std::string name;
    int playerNb = 3;
    int botBehaviour = -1;
    Trainer *player1 = nullptr, *player2 = nullptr;

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(0, 151);

    std::ifstream titleScreen("gfx/title.txt");

    if (titleScreen) {
        std::string line;

        while (std::getline(titleScreen, line)) {
            std::cout << line << std::endl;
        }

        titleScreen.close();
    }
    while (std::cin.get()!='\n');

    switch(gameMenu()){
        case 0:

            while(playerNb != 0 && playerNb != 1 && playerNb != 2){
                std::cout << "Select players number[0-2]: " << std::endl;
                std::cin >> playerNb;
                CLEAR;
            }
            if(playerNb < 2){
                while(botBehaviour != 0 && botBehaviour != 1){
                    std::cout << "Choose a behavior for bots" << std::endl << "[0]: Random" << std::endl << "[1]: Mindful" << std::endl;
                    std::cin >> botBehaviour;
                    CLEAR;
                }

            }
            if(playerNb > 0){
                std::cout << "Enter name for player 1: " << std::endl;
                std::cin >> name;
                player1 = new Trainer(name);
                player1->setIsHuman(true);
                CLEAR;
            }else player1 = new Trainer("CPU1");

            if(playerNb > 1){
                std::cout << "Enter name for player 2: " << std::endl;
                std::cin >> name;
                player2 = new Trainer(name);
                player2->setIsHuman(true);
                CLEAR;
            }else player2 = new Trainer("CPU2");

            for(int i = 0; i < 6; i++){
                player1->catchPokemon(distr(eng));
                player2->catchPokemon(distr(eng));
            }
            break;
        case 1:
        std::string name;
        char secondPlayer;
        std::cout << "Enter the player's name :";
        std::cin >> name;
        player1 = player1->loadTrainer(name);
        player1->setIsHuman(true);
        std::cout << "Do you want to load another profile for player 2 ? [Y/N]: ";
        std::cin >> secondPlayer;

        if(toupper(secondPlayer) == 'Y'){
            std::cout << "Enter the player's name :";
            std::cin >> name;
            player2 = player2->loadTrainer(name);
            player2->setIsHuman(true);
        }
        else{
            player2 = new Trainer("CPU");
            for(int i = 0; i < 6; i++){
                player2->catchPokemon(distr(eng));
            }
            while(botBehaviour != 0 && botBehaviour != 1){
                std::cout << "Choose a behavior for bots" << std::endl << "[0]: Random" << std::endl << "[1]: Mindful" << std::endl;
                std::cin >> botBehaviour;
                CLEAR;
            }
        }
    }

    #if defined __unix__ || __APPLE__
    while (std::cin.get()!='\n');
    #endif

    while(true){
        if(playerNb > 0 && player1->getIsHuman() == true) player1->interBattleMenu(0);
        if(playerNb > 1 && player2->getIsHuman() == true) player2->interBattleMenu(0);
        if (player1->getItsLevel() > player2->getItsLevel() ||
            (player1->getItsLevel() == player2->getItsLevel() &&
             player1->getItsXp() > player2->getItsXp())) {
            if(player1->getIsHuman() == true) player1->choosePokemon();
            else player1->botChoosePokemon(player2->getItsTeam().at(0), botBehaviour);
            if(player2->getIsHuman() == true) player2->choosePokemon();
            else player2->botChoosePokemon(player1->getItsTeam().at(0), botBehaviour);

        } else if (player1->getItsLevel() < player2->getItsLevel() ||
                   (player1->getItsLevel() == player2->getItsLevel() &&
                    player1->getItsXp() < player2->getItsXp())) {
            if(player2->getIsHuman() == true) player2->choosePokemon();
            else player2->botChoosePokemon(player1->getItsTeam().at(0), botBehaviour);
            if(player1->getIsHuman() == true) player1->choosePokemon();
            else player1->botChoosePokemon(player2->getItsTeam().at(0), botBehaviour);
        } else {
            std::random_device rd;
            std::default_random_engine eng(rd());
            std::uniform_int_distribution<int> distr(1, 2);

            if (distr(eng) == 1) {
                if(player1->getIsHuman() == true) player1->choosePokemon();
                else player1->botChoosePokemon(player2->getItsTeam().at(0), botBehaviour);
                if(player2->getIsHuman() == true) player2->choosePokemon();
                else player2->botChoosePokemon(player1->getItsTeam().at(0), botBehaviour);
            } else {
                if(player2->getIsHuman() == true) player2->choosePokemon();
                else player2->botChoosePokemon(player1->getItsTeam().at(0), botBehaviour);
                if(player1->getIsHuman() == true) player1->choosePokemon();
                else player1->botChoosePokemon(player2->getItsTeam().at(0), botBehaviour);
            }
        }
        displayBattle(player1, player2);
        while (!player1->hasTrainerLost() && !player2->hasTrainerLost()) {
            Trainer* first;
            Trainer* second;

            // Determine turn order
            if (player1->getItsTeam().at(0)->getItsSpeed() > player2->getItsTeam().at(0)->getItsSpeed()) {
                first = player1;
                second = player2;
            } else if (player1->getItsTeam().at(0)->getItsSpeed() < player2->getItsTeam().at(0)->getItsSpeed()) {
                first = player2;
                second = player1;
            } else {
                // Coin flip for tie-breaker
                std::random_device rd;
                std::default_random_engine eng(rd());
                std::uniform_int_distribution<int> coinFlip(0, 1);

                if (coinFlip(eng) == 0) {
                    first = player1;
                    second = player2;
                } else {
                    first = player2;
                    second = player1;
                }
            }

            // First Pokémon attacks
            attack(first, second);
            #ifdef _WIN32
            updateDisplayBattle(player1, player2);
            #else
            displayBattle(player1, player2);
            #endif

            // Check if the second Pokémon fainted
            if (second->getItsTeam().at(0)->getItsHp() <= 0) {
                std::cout << second->getItsTeam().at(0)->getItsName() << " fainted!\n" << std::endl;
                while (std::cin.get() != '\n');
                if (second->getIsHuman()) {
                    second->choosePokemon();
                    displayBattle(player1, player2);
                } else {
                    second->botChoosePokemon(first->getItsTeam().at(0), botBehaviour);
                    displayBattle(player1, player2);
                }
                continue; // Restart turn with new Pokémon
            }

            // Second Pokémon retaliates
            attack(second, first);
            #ifdef _WIN32
            updateDisplayBattle(player1, player2);
            #else
            displayBattle(player1, player2);
            #endif

            // Check if the first Pokémon fainted
            if (first->getItsTeam().at(0)->getItsHp() <= 0) {
                std::cout << first->getItsTeam().at(0)->getItsName() << " fainted!\n" << std::endl;
                while (std::cin.get() != '\n');
                if (first->getIsHuman()) {
                    first->choosePokemon();
                    displayBattle(player1, player2);
                } else {
                    first->botChoosePokemon(second->getItsTeam().at(0), botBehaviour);
                    displayBattle(player1, player2);
                }
            }
        }

        if(!player1->hasTrainerLost()) std::cout << player1->getItsName() << " has won the battle !" << std::endl;
        else std::cout << player2->getItsName() << " has won the battle !" << std::endl;
        while (std::cin.get()!='\n');
        if(player1->getIsHuman() == true) player1->saveTrainer();
        if(player2->getIsHuman() == true) player2->saveTrainer();
        while (std::cin.get()!='\n');
        CLEAR;

        player1->healTeam();
        player2->healTeam();
   }
}

void Game::displayBattle(Trainer *player1, Trainer *player2)
{
    CLEAR;
    std::cout << "+====================+" << std::endl;
    std::cout << "| " << player2->getItsTeam().at(0)->getItsName();
    for(int i = 18 - player2->getItsTeam().at(0)->getItsName().length(); i >= 0; i--){
        std::cout << " ";
    }
    std::cout << "|" << std::endl;
    std::string hp = "| HP:" + std::to_string(player2->getItsTeam().at(0)->getItsHp()) + "/" + std::to_string(player2->getItsTeam().at(0)->getItsMaxHp());
    std::cout << hp;
    for(int i = 20 - hp.length(); i >= 0; i--){
        std::cout << " ";
    }
    std::cout << "|" << std::endl;
    std::cout << "+====================+" << std::endl;

    std::ifstream pkmnSprite("gfx/" + player2->getItsTeam().at(0)->getItsSprite() + ".txt");

    if (pkmnSprite) {
        std::string line;

        while (std::getline(pkmnSprite, line)) {
            std::cout << "    " << line << std::endl;
        }

        pkmnSprite.close();
    } else {
        std::cout << "Invalid sprite for pokemon " << player2->getItsTeam().at(0)->getItsName() << std::endl;
    }

    std::cout << std::endl << std::endl;

    std::ifstream pkmnSprite2("gfx/" + player1->getItsTeam().at(0)->getItsSprite() + ".txt");

    if (pkmnSprite2) {
        std::string line;

        while (std::getline(pkmnSprite2, line)) {
            std::cout << "    " << line << std::endl;
        }

        pkmnSprite2.close();
    } else {
        std::cout << "Invalid sprite for pokemon " << player1->getItsTeam().at(0)->getItsName() << std::endl;
    }


std::cout << "+====================+" << std::endl;
std::cout << "| " << player1->getItsTeam().at(0)->getItsName();
for(int i = 18 - player1->getItsTeam().at(0)->getItsName().length(); i >= 0; i--){
        std::cout << " ";
}
std::cout << "|" << std::endl;
hp = "| HP:" + std::to_string(player1->getItsTeam().at(0)->getItsHp()) + "/" + std::to_string(player1->getItsTeam().at(0)->getItsMaxHp());
std::cout << hp;
for(int i = 20 - hp.length(); i >= 0; i--){
        std::cout << " ";
}
std::cout << "|" << std::endl;
std::cout << "+====================+" << std::endl;


}

void Game::updateDisplayBattle(Trainer *player1, Trainer *player2)
{
    std::cout << "\033[3;0H"; //places the cursor back to line 3
    std:: cout << "\33[2K"; //Erases the line the cursor is currently on
    std::string hp = "| HP:" + std::to_string(player2->getItsTeam().at(0)->getItsHp()) + "/" + std::to_string(player2->getItsTeam().at(0)->getItsMaxHp());
    std::cout << hp;
    for(int i = 20 - hp.length(); i >= 0; i--){
        std::cout << " ";
    }
    std::cout << "|" << std::endl;

    std::cout << "\033[29;0H"; //places the cursor back to line 29
    std:: cout << "\33[2K"; //Erases the line the cursor is currently on
    hp = "| HP:" + std::to_string(player1->getItsTeam().at(0)->getItsHp()) + "/" + std::to_string(player1->getItsTeam().at(0)->getItsMaxHp());
    std::cout << hp;
    for(int i = 20 - hp.length(); i >= 0; i--){
        std::cout << " ";
    }
    std::cout << "|" << std::endl;

    std::cout << "\033[33;0H"; //places the cursor back to line 33
    std:: cout << "\33[2K"; //Erases the line the cursor is currently on
    std::cout << "\033[32;0H"; //places the cursor back to line 32
    std:: cout << "\33[2K"; //Erases the line the cursor is currently on
    std::cout << "\033[31;0H"; //places the cursor back to line 31
    std:: cout << "\33[2K"; //Erases the line the cursor is currently on
}

void Game::redrawPokemon(Trainer* player1, Trainer* player2)
{
    if(player2->getItsTeam().at(0)->getItsHp() == player2->getItsTeam().at(0)->getItsMaxHp()){
        if(player2->getIsHuman() == true) displayBattle(player1, player2);
        else{
            std::cout << "\033[2;0H"; //places the cursor back to line 2
            std:: cout << "\33[2K"; //Erases the line the cursor is currently on
            std::cout << "| " << player2->getItsTeam().at(0)->getItsName();
            for(int i = 18 - player2->getItsTeam().at(0)->getItsName().length(); i >= 0; i--){
                std::cout << " ";
            }
            std::cout << "|" << std::endl;
            std::cout << "\033[4;0H"; //places the cursor back to line 4
            for(int i = 10; i > 0; i--){
                std::cout << "\033[B"; //Moves the cursor down one line
                std:: cout << "\33[2K"; //Erases the line the cursor is currently on
            }
            std::cout << "\033[5;0H"; //places the cursor back to line 5

            std::ifstream pkmnSprite("gfx/" + player2->getItsTeam().at(0)->getItsSprite() + ".txt");

            if (pkmnSprite) {
                std::string line;

                while (std::getline(pkmnSprite, line)) {
                    std::cout << "    " << line << std::endl;
                }
                pkmnSprite.close();
            } else {
                std::cout << "Invalid sprite for pokemon " << player2->getItsTeam().at(0)->getItsName() << std::endl;
            }
            updateDisplayBattle(player1, player2);

            std::cout << "\033[35;0H"; //places the cursor back to line 35
            std:: cout << "\33[2K"; //Erases the line the cursor is currently on
            std::cout << "\033[34;0H"; //places the cursor back to line 34
            std:: cout << "\33[2K"; //Erases the line the cursor is currently on
            std::cout << "\033[33;0H"; //places the cursor back to line 33
            std:: cout << "\33[2K"; //Erases the line the cursor is currently on
            std::cout << "\033[32;0H"; //places the cursor back to line 32
            std:: cout << "\33[2K"; //Erases the line the cursor is currently on
            std::cout << "\033[31;0H"; //places the cursor back to line 31
            std:: cout << "\33[2K"; //Erases the line the cursor is currently on
        }
    }
    if(player1->getItsTeam().at(0)->getItsHp() == player1->getItsTeam().at(0)->getItsMaxHp()){
        if(player1->getIsHuman() == true) displayBattle(player1, player2);
        else{
            std::cout << "\033[28;0H"; //places the cursor back to line 28
            std:: cout << "\33[2K"; //Erases the line the cursor is currently on
            std::cout << "| " << player1->getItsTeam().at(0)->getItsName();
            for(int i = 18 - player1->getItsTeam().at(0)->getItsName().length(); i >= 0; i--){
                std::cout << " ";
            }
            std::cout << "|" << std::endl;
            std::cout << "\033[15;0H"; //places the cursor back to line 15
            for(int i = 10; i > 0; i--){
                std::cout << "\033[B"; //Moves the cursor down one line
                std:: cout << "\33[2K"; //Erases the line the cursor is currently on
            }
            std::cout << "\033[16;0H"; //places the cursor back to line 16

            std::ifstream pkmnSprite("gfx/" + player1->getItsTeam().at(0)->getItsSprite() + ".txt");

            if (pkmnSprite) {
                std::string line;

                while (std::getline(pkmnSprite, line)) {
                    std::cout << "    " << line << std::endl;
                }

                pkmnSprite.close();
            } else {
                std::cout << "Invalid sprite for pokemon " << player1->getItsTeam().at(0)->getItsName() << std::endl;
            }
            updateDisplayBattle(player1, player2);

            std::cout << "\033[35;0H"; //places the cursor back to line 35
            std:: cout << "\33[2K"; //Erases the line the cursor is currently on
            std::cout << "\033[34;0H"; //places the cursor back to line 34
            std:: cout << "\33[2K"; //Erases the line the cursor is currently on
            std::cout << "\033[33;0H"; //places the cursor back to line 33
            std:: cout << "\33[2K"; //Erases the line the cursor is currently on
            std::cout << "\033[32;0H"; //places the cursor back to line 32
            std:: cout << "\33[2K"; //Erases the line the cursor is currently on
            std::cout << "\033[31;0H"; //places the cursor back to line 31
            std:: cout << "\33[2K"; //Erases the line the cursor is currently on
        }
    }

}

void Game::attack(Trainer *attacker, Trainer *defender)
{

    if(defender->getItsTeam().at(0)->getItsHp() > 0 && attacker->getItsTeam().at(0)->getItsHp() > 0){

        // Prepare random engine for the random value
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_real_distribution<float> distr(0.850980392, 1.0); // 217 - 255, then divided by 255

        int level = 50; // To be implemented
        int basePower = 50; // To be implemented
        int atk = attacker->getItsTeam().at(0)->getItsAtk();
        int def = defender->getItsTeam().at(0)->getItsDef();
        float STAB = 1.5; // To be implemented
        float typeEfficacity = attacker->getItsTeam().at(0)->typeEfficacity(attacker->getItsTeam().at(0)->getItsType(), defender->getItsTeam().at(0)->getItsType());
        float random = distr(eng);

        int damage = ((((2*level/5 + 2)*atk*basePower/def)/50)+2)*random*STAB*typeEfficacity;

        std::cout << attacker->getItsTeam().at(0)->getItsName() << " uses " << "[attack]" << " !" << std::endl; // TODO : Add attack name
        if(typeEfficacity == 0) std::cout << "It doesn't affect " << defender->getItsTeam().at(0)->getItsName() << " !" << std::endl;
        else if(typeEfficacity == 0.5) std::cout << "It's not very effective !" << std::endl;
        else if(typeEfficacity == 2) std::cout << "It's very effective !" << std::endl;

        // Give xp if the attack is a one hit kill
        if(defender->getItsTeam().at(0)->getItsHp() == defender->getItsTeam().at(0)->getItsMaxHp() && damage >= defender->getItsTeam().at(0)->getItsHp()){
            attacker->gainXp(3);
            defender->looseXp(3);
        }
        defender->getItsTeam().at(0)->takeDamage(damage);
        while (std::cin.get()!='\n');
    }
}

int Game::gameMenu(int selected){
    initscr(); // Initialize the ncurses library
    cbreak(); // Enable cbreak mode
    noecho(); // Disable echoing of input
    keypad(stdscr, TRUE); // Enable keypad mode

    while (true) {
        clear(); // Clear the screen
        if (selected == 0)
            printw("> New Game  ");
        else
            printw("  New Game  ");

        if (selected == 1)
            printw("> Load Game  ");
        else
            printw("  Load Game  ");

        refresh(); // Update the screen

        int ch = getch();
        switch (ch){
            case KEY_LEFT:
                if (selected > 0) selected--;
                break;
            case KEY_RIGHT:
                if (selected < 1) selected++;
                break;
            case ENTER:
                switch (selected) {
                case 0:
                    endwin(); // Clean up the ncurses library
                    return 0;
                case 1:
                    endwin(); // Clean up the ncurses library
                    return 1;
                default:
                    break;
                }
                break;
            default:
                break;
        }
    }
}







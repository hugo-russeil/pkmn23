#include <termios.h>
#include <unistd.h>
#include <curses.h>
#define CLEAR system("clear")
#define ENTER 10

#include "game.h"
#include "move.h"
#include "pokemon.h"
#include <random>
#include <fstream>
#include <chrono>
#include <thread>

Game::Game()
{

}

Trainer *player1 = nullptr, *player2 = nullptr;

void Game::gameLoop()
{
    std::string name;
    int playerNb = 3;

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
        }
    }

    #if defined __unix__ || __APPLE__
    while (std::cin.get()!='\n');
    #endif

    while(true){
        if(playerNb > 0 && player1->getIsHuman() == true) player1->interBattleMenu(0);
        if(playerNb > 1 && player2->getIsHuman() == true) player2->interBattleMenu(0);
        

        // Send the first Pokémon from each player's team into battle
        player1->sendToBattle(0);
        player2->sendToBattle(0);

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
                    second->botChoosePokemon(first->getItsTeam().at(0));
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
                    first->botChoosePokemon(second->getItsTeam().at(0));
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
    Move move;

    if(attacker->getIsHuman()) move= selectMove(*attacker->getItsTeam().at(0));
    else move = botSelectMove(*attacker->getItsTeam().at(0), defender);

    if(defender->getItsTeam().at(0)->getItsHp() > 0 && attacker->getItsTeam().at(0)->getItsHp() > 0){

        std::cout << attacker->getItsTeam().at(0)->getItsName() << " uses " << move.getName() << " !" << std::endl;

        // Check if the move misses and return immediately if it does
        if(move.getAccuracy() < 100){
            std::random_device rd;
            std::default_random_engine eng(rd());
            std::uniform_int_distribution<int> distr(1, 100);
            int accuracy = distr(eng);
            if(accuracy > move.getAccuracy()){
                std::cout << "But misses !" << std::endl;
                while (std::cin.get()!='\n');
                return;
            }
        }

        // Return immediately if the move doesn't affect the target
        float typeEfficacity = computeTypeEfficacity(move.getType(), defender->getItsTeam().at(0)->getItsType());
        if(typeEfficacity == 0){ 
            std::cout << "It doesn't affect " << defender->getItsTeam().at(0)->getItsName() << " !" << std::endl;
            while (std::cin.get()!='\n');
            return;
        }
        else if(typeEfficacity == 0.5) std::cout << "It's not very effective !" << std::endl;
        else if(typeEfficacity == 2) std::cout << "It's very effective !" << std::endl;

        // Prepare random engine for the random value
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_real_distribution<float> distr(0.850980392, 1.0); // 217 - 255, then divided by 255

        int level = 50; // To be implemented
        int basePower = move.getBasePower();
        int atk = (move.getCategory() == Move::Category::Physical) ? attacker->getItsTeam().at(0)->getItsAtk() : attacker->getItsTeam().at(0)->getItsSpecial();
        int def = (move.getCategory() == Move::Category::Physical) ? defender->getItsTeam().at(0)->getItsDef() : defender->getItsTeam().at(0)->getItsSpecial();
        float STAB = (attacker->getItsTeam().at(0)->getItsType() == move.getType()) ? 1.5 : 1;
        float random = distr(eng);

        int damage = ((((2*level/5 + 2)*atk*basePower/def)/50)+2)*random*STAB*typeEfficacity;

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


#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <vector>

Move Game::selectMove(Pokemon &pokemon) {
    const auto &moves = pokemon.getItsMoves();
    int moveCount = moves.size();
    if (moveCount == 0) {
        throw std::runtime_error("This Pokémon has no moves!");
    }

    int currentChoice = 0;         // Current selected move
    const int moveColumns = 2;     // Number of moves per line
    const int moveRows = (moveCount + 1) / moveColumns; // Calculate number of rows

    // Save original terminal settings
    struct termios original, raw;
    tcgetattr(STDIN_FILENO, &original); // Get current settings
    raw = original;

    // Disable canonical mode and echo
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    try {
        while (true) {
            // Clear and redisplay the battle information
            CLEAR;
            displayBattle(player1, player2);

            // Display the move selection menu
            std::cout << "\nSelect a move for " << pokemon.getItsName() << ":\n";

            // Display moves in a grid (2 moves per row)
            for (int i = 0; i < moveCount; ++i) {
                int col = i % moveColumns;

                // Add indentation for the second column
                if (col == 1) {
                    std::cout << "     ";
                }

                // Highlight the selected move with '>'
                if (i == currentChoice) {
                    std::cout << "> " << moves[i].getName() << "   ";
                } else {
                    std::cout << "  " << moves[i].getName() << "   ";
                }

                // Add newline at the end of each row
                if (col == 1) {
                    std::cout << "\n";
                }
            }

            // Wait for user input
            int input = getch();

            // Handle navigation and selection
            switch (input) {
            case KEY_UP:
                if (currentChoice - moveColumns >= 0) {
                    currentChoice -= moveColumns; // Move up
                }
                break;
            case KEY_DOWN:
                if (currentChoice + moveColumns < moveCount) {
                    currentChoice += moveColumns; // Move down
                }
                break;
            case KEY_LEFT:
                if (currentChoice > 0) {
                    currentChoice--; // Move left
                }
                break;
            case KEY_RIGHT:
                if (currentChoice < moveCount - 1) {
                    currentChoice++; // Move right
                }
                break;
            case '\n': // Enter key (Confirm selection)
                CLEAR;
                displayBattle(player1, player2);
                return moves[currentChoice];
            default:
                break;
            }
        }
    } catch (...) {
        // Restore original terminal settings in case of exceptions
        tcsetattr(STDIN_FILENO, TCSANOW, &original);
        throw;
    }

    // Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &original);
}

Move Game::botSelectMove(Pokemon &pokemon, Trainer *target) {
    std::vector<Move> moves = pokemon.getItsMoves();

    if (moves.empty()) {
        // Handle the case when the Pokémon has no moves
        // You can either throw an exception or return a default move
        throw std::runtime_error("This Pokémon has no moves!");
    }

    // Search for a move with a type advantage over the opponent
    for (const auto &move : moves) {
        if (computeTypeEfficacity(move.getType(), target->getItsTeam().at(0)->getItsType()) == 2) {
            return move;
        }
    }

    // If no move with a type advantage is found, search for a move with no type weakness
    for (const auto &move : moves) {
        if (computeTypeEfficacity(move.getType(), target->getItsTeam().at(0)->getItsType()) == 1) {
            return move;
        }
    }

    // If no move with a type advantage or no type weakness is found, return the first move
    return moves[0];
}
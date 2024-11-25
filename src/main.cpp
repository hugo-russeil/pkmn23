#if defined(WIN32)
#include <Windows.h>
#define CLEAR system("cls")
#elif __unix__ || __APPLE__
#define CLEAR system("clear")
#endif

#include "pokemon.h"
#include "game.h"
#include <fstream>
#include <iostream>
#include <random>


int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    system("color 70");
#endif
    std::cout << "\e[8;40;50t";
    std::cout << "\u001b[?25l"; //hides the cursor in the console
    Game* game = new Game();
    game->gameLoop();
    return 0;
}

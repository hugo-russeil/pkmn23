# **PKMN23**

PKMN23 is a turn-based Pokémon battle game designed to run in the terminal, inspired by classic battles from the original Game Boy Pokémon games. Initially created as a university project to demonstrate object-oriented programming (OOP) principles, this version has been reworked to remove unnecessary constraints from the original assignment, making it actually playable.  

While the game is still incomplete, it features all 151 first-generation Pokémon, type matchups, and basic move functionality, with plans to expand upon battle mechanics in future updates.  


## **Features**  
- Turn-based battles, similar to the Game Boy Pokémon games  
- Full roster of 151 first-generation Pokémon  
- Local multiplayer support  
- Single-player mode with AI battles (including AI-only matches)  
- Basic move functionality (more moves to be added)  
- Type effectiveness system  

**Upcoming Features:**  
- Status effects (e.g., paralysis, sleep, poison)  
- More moves from the original games  


## **Installation & Running the Game**  
### **Requirements**  
- A C++ compiler (g++)  
- `ncurses` library  

### **Build & Run**  
Clone the repository:  
```bash
git clone https://github.com/yourusername/PKMN23.git
cd PKMN23
```
Compile the game using the provided Makefile:  
```bash
make
```
Run the game:  
```bash
./pkmn23
```
To clean up compiled files:  
```bash
make clean
```

## **Tech Stack**  
- **Language:** C++ (C++11 standard)  
- **Libraries:** ncurses (for terminal UI)  
- **Build System:** Makefile  


## **License**

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details. 

## **Disclaimer**

This is a fan-made project created for educational and entertainment purposes. Pokémon and all related trademarks are the property of Nintendo, Game Freak, and The Pokémon Company. I do not claim ownership of any official Pokémon assets, and this project is not affiliated with or endorsed by the official Pokémon franchise. Please don’t sue me.

#ifndef GLOBAL_H
#define GLOBAL_H 1

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class Instruction;
class Map;
class User;
class Bag;
class Monster;
class Fight;

namespace globalVar {
    // var
    std::ifstream file_in;
    std::ofstream file_out;
    std::stringstream stream;
    nlohmann::json jin;
    bool genMons = true;
    bool gameState = true;
    std::vector<Monster*> monsterList;
    std::vector<double> chance_m;
    int killNum = 0;

    // class
    User* user = nullptr;
    Map* screen = nullptr;
    Instruction* Command = nullptr;
    Bag* bg = nullptr;
    Fight* ft = nullptr;
};

#endif
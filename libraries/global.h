#ifndef GLOBAL_H
#define GLOBAL_H 1

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class Instruction;
class Map;
class User;

namespace globalVar {
    // var
    std::ifstream file_in;
    std::ofstream file_out;
    std::stringstream stream;
    
    // class
    User* user = nullptr;
    Map* screen = nullptr;
    Instruction* Command = nullptr;
};

#endif
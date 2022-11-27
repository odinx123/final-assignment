#ifndef GLOBAL_H
#define GLOBAL_H 1

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class Instruction;
class Map;

namespace globalVar {
    // var
    std::fstream file_in;
    std::stringstream stream;
    
    // class
    Map* screen;
    Instruction* Command;
};

#endif
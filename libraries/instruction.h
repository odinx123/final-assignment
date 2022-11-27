#ifndef INSTRUCTION_H
#define INSTRUCTION_H 1

#include <string>
#include <unordered_map>

#include "map.h"

typedef int (*insFun)(const std::vector<std::string>&);

class Instruction {
   private:
    std::unordered_map<std::string, void*> funcMap;

   public:
    Instruction();

    int insertCommand();
    void splitstring(const std::string& s, std::vector<std::string>& v, const std::string& c);
};

int funcExit(const std::vector<std::string>& tokens) {
    return -1;
}

int funcMove(const std::vector<std::string>& tokens) {
    if (tokens.size() != 1 && tokens.size() != 2) {
        globalVar::screen->setColor(4);
        globalVar::screen->setMes("指令錯誤!!!", 0, 0);
        globalVar::screen->setColor();
        return 1;
    }

    if (tokens.size() == 1) {
        int i;
        globalVar::screen->setInfoCursorPos(0, 4);
        std::string mes = "請輸入指令:";
        std::cout << mes << std::endl;

        globalVar::screen->showCityNumbering(30, 1);

        globalVar::screen->setCursorVisible(true);
        (std::cin >> i).get();
        globalVar::screen->setCursorVisible(false);

        if (std::cin.fail()) {
            globalVar::screen->setColor(4);
            globalVar::screen->setMes("This map does not exist.", 0, 0);
            globalVar::screen->setColor();
        }

        globalVar::screen->clearMes(0, 5, 10);
        globalVar::screen->clearMes(0, 4, mes.size());
        globalVar::screen->clearCityNumbering(30, 1);

        if (std::cin.fail()) {
			fflush(stdin);
            std::cin.clear();
            return 1;
        } else
            globalVar::screen->loadMap(globalVar::screen->getEngCityName(i));
    } else {
        try {
        	globalVar::screen->loadMap(globalVar::screen->getEngCityName(std::stoi(tokens[1])));
        } catch (const std::invalid_argument& e) {
            globalVar::screen->setColor(4);
			globalVar::screen->setMes("This map does not exist.", 0, 0);
            globalVar::screen->setColor();
            return 1;
        }

    }

    return 0;
}

Instruction::Instruction() {
    funcMap["exit"] = reinterpret_cast<void*>(funcExit);
    funcMap["move"] = reinterpret_cast<void*>(funcMove);
}

int Instruction::insertCommand() {
    std::string sentense;

    globalVar::screen->clearMes(0, 0, 30);
    globalVar::screen->setInfoCursorPos(0, 4);
    std::string mes = "請輸入指令:";
    std::cout << mes << std::endl;
    globalVar::screen->setCursorVisible(true);

    std::getline(std::cin, sentense);
    std::vector<std::string> tokens;
    splitstring(sentense, tokens, " ");
    if (tokens.size() == 0) {
		globalVar::screen->clearMes(0, 4, mes.size());
		globalVar::screen->clearMes(0, 5, sentense.size());
        globalVar::screen->setCursorVisible(false);
		return 0;
	}

    auto it = funcMap.find(tokens[0]);
    if (it == funcMap.end()) {
        globalVar::screen->setColor(4);
        globalVar::screen->setMes("指令錯誤!!!", 0, 0);
        globalVar::screen->setColor();
		globalVar::screen->clearMes(0, 4, mes.size());
    	globalVar::screen->clearMes(0, 5, sentense.size());
        return 0;
    }

    globalVar::screen->clearMes(0, 4, mes.size());
    globalVar::screen->clearMes(0, 5, sentense.size());
    globalVar::screen->setCursorVisible(false);
    return (reinterpret_cast<insFun>(it->second)(tokens));
}

void Instruction::splitstring(const std::string& s, std::vector<std::string>& v, const std::string& c) {
    std::string::size_type pos1, pos2;
    pos2 = s.find_first_of(c);
    pos1 = 0;
    while (pos2 != std::string::npos) {
        if (pos2 - pos1)
            v.push_back(s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = s.find_first_of(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

#endif
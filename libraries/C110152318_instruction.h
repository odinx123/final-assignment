#ifndef INSTRUCTION_H
#define INSTRUCTION_H 1

#include <string>
#include <unordered_map>

#include "C110152318_map.h"
#include "C110152318_bag.h"

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
    globalVar::user->saveData();
    return -1;
}

int funcSave(const std::vector<std::string>& tokens) {
    globalVar::user->saveData();
    globalVar::screen->clearMes(0, 0, 30);
    globalVar::screen->setColor(10);
    globalVar::screen->setMes("Already save data.", 0, 0);
    globalVar::screen->setColor();
    return 0;
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

        globalVar::screen->showCityNumbering();

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

int funcShop(const std::vector<std::string>& tokens) {
    if (globalVar::screen->getCurCity() != "market") {
        globalVar::screen->clearMes(0, 0, 30);
        globalVar::screen->setColor(4);
        globalVar::screen->setMes("you can't buy things here!!!", 0, 0);
        globalVar::screen->setColor();
        return 1;
    }
    globalVar::screen->printMapMes("Shop List:");
    globalVar::bg->showShopList();

    std::string ins;
    globalVar::screen->setCursorVisible(true);
    auto orgPos = globalVar::screen->getCurMesPos();
    (std::cin >> ins).get();
    globalVar::screen->clearMes(orgPos.X, orgPos.Y, 10);
    globalVar::screen->setCursorVisible(false);
    if (ins.size() == 0 || ins == "exit")
        return 0;
    
    globalVar::bg->buy(stoi(ins));
    globalVar::screen->printMapMes(" ");

    return 0;
}

int funcShowBag(const std::vector<std::string>& tokens) {
    globalVar::screen->printMapMes("Bag List:");
    globalVar::bg->showBagList();
    globalVar::screen->printMapMes(" ");

    return 0;
}

Instruction::Instruction() {
    funcMap["exit"] = reinterpret_cast<void*>(funcExit);
    funcMap["move"] = reinterpret_cast<void*>(funcMove);
    funcMap["save"] = reinterpret_cast<void*>(funcSave);
    funcMap["shop"] = reinterpret_cast<void*>(funcShop);
    funcMap["bag"] = reinterpret_cast<void*>(funcShowBag);
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
        globalVar::screen->setCursorVisible(false);
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
#ifndef INSTRUCTION_H
#define INSTRUCTION_H 1

#include <string>
#include <unordered_map>

#include "C110152318_bag.h"
#include "C110152318_map.h"
#include "C110152318_monster.h"
#include "C110152318_monstData.h"
#include "C110152318_fight.h"

typedef int (*insFun)(const std::vector<std::string>&);

class Instruction {
   private:
    std::unordered_map<std::string, void*> funcMap;

   public:
    Instruction();

    int insertCommand();
    void splitstring(const std::string& s, std::vector<std::string>& v, const std::string& c);
    void printConmand() const;
};

inline void printMes(const std::string& mes, SHORT x, SHORT y, int color);

int funcExit(const std::vector<std::string>& tokens) {
    return -1;
}

int funcSave(const std::vector<std::string>& tokens) {
    globalVar::user->saveData();
    globalVar::bg->saveBagItem();
    globalVar::screen->clearMes(0, 0, 30);
    printMes("Already save data.", 0, 0, 10);
    return 0;
}

int funcMove(const std::vector<std::string>& tokens) {
    if (tokens.size() != 1 && tokens.size() != 2) {
        printMes("指令錯誤!!!", 0, 0, 4);
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

        if (std::cin.fail())
            printMes("This map does not exist.", 0, 0, 4);

        globalVar::screen->clearMes(0, 5, 10);
        globalVar::screen->clearMes(0, 4, mes.size());

        if (std::cin.fail()) {
            fflush(stdin);
            std::cin.clear();
            return 1;
        }
        globalVar::screen->loadMap(globalVar::screen->getEngCityName(i));
    } else {
        try {
            globalVar::screen->loadMap(globalVar::screen->getEngCityName(std::stoi(tokens[1])));
        } catch (const std::invalid_argument& e) {
            printMes("This map does not exist.", 0, 0, 4);
            return 1;
        }
    }

    return 0;
}

int funcShop(const std::vector<std::string>& tokens) {
    if (globalVar::screen->getCurCity() != "market") {
        globalVar::screen->clearMes(0, 0, 30);
        printMes("you can't buy things here!!!", 0, 0, 4);
        return 1;
    } else if (tokens.size() > 1) {
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }
    globalVar::screen->printMapMes("Shop List:");
    globalVar::bg->showShopList();
    globalVar::screen->printMapMes(" ");

    std::string ins;
    globalVar::screen->setCursorVisible(true);
    globalVar::screen->setInfoCursorPos(0, 5);
    std::getline(std::cin, ins);
    if (ins.size() == 0) {
        globalVar::screen->clearMes(0, 5, 10);
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }
    globalVar::screen->clearMes(0, 5, 10);
    globalVar::screen->setCursorVisible(false);
    if (ins.size() == 0 || ins == "exit")
        return 0;

    int index;
    try {
        index = stoi(ins);
    } catch (const std::invalid_argument& e) {
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }

    // todo
    auto state = globalVar::bg->buy(index);
    if (state == 1) {
        globalVar::screen->printMapMes("錢不夠購買" + globalVar::bg->getShopNameByIndex(index));
    } else if (state == 2) {
        globalVar::screen->printMapMes("指令錯誤，請輸入數字!!!");
    } else if (state == 3) {
        globalVar::screen->printMapMes("背包大小不足!!!");
    } else if (state == 4) {
        globalVar::screen->printMapMes("職業不合，無法購買!!!");
    }
    globalVar::screen->printMapMes(" ");

    return 0;
}

int funcShowBag(const std::vector<std::string>& tokens) {
    if (tokens.size() > 2) {
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }
    globalVar::screen->printMapMes("Bag List:");
    globalVar::bg->showBagList();
    globalVar::screen->printMapMes(" ");
    if (!globalVar::bg->isBagCanUse())
        return 1;

    if (tokens.size() == 2 && (tokens[1] == "use" || tokens[1] == "deuse")) {
        std::string ins;
        int index;

        globalVar::screen->setInfoCursorPos(0, 5);
        globalVar::screen->setCursorVisible(true);
        std::getline(std::cin, ins);
        globalVar::screen->setCursorVisible(false);
        globalVar::screen->clearMes(0, 5, 10);
        if (ins.size() == 0) {
            globalVar::screen->clearMes(0, 5, 10);
            globalVar::screen->clearMes(0, 0, 30);
            printMes("Command error!!!", 0, 0, 4);
            return 1;
        }
        try {
            index = stoi(ins);
        } catch (const std::invalid_argument& e) {
            globalVar::screen->clearMes(0, 0, 30);
            printMes("Command error!!!", 0, 0, 4);
            return 1;
        }

        if (tokens[1] == "use") {
            if (globalVar::user->ware && globalVar::bg->getTypeByIdx(index) == 1) {
                globalVar::screen->clearMes(0, 0, 30);
                printMes("請脫下裝備再穿著!!!", 0, 0, 4);
                return 1;
            }
            auto t = globalVar::bg->useBagItem(index, true);
        } else if (tokens[1] == "deuse") {
            if (!globalVar::user->ware) {
                globalVar::screen->clearMes(0, 0, 30);
                printMes("未穿著裝備!!!", 0, 0, 4);
                return 1;
            }
            auto t = globalVar::bg->useBagItem(index, false);
        }
    }

    return 0;
}

int funcHelp(const std::vector<std::string>& tokens) {
    if (tokens.size() > 1) {
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }

    globalVar::screen->printMapMes("Instruction List:");
    globalVar::Command->printConmand();
    return 0;
}

int funcMonsList(const std::vector<std::string>& tokens) {
    if (tokens.size() > 1) {
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }
    // printMonstList
    globalVar::screen->printMapMes("怪物資訊:");
    globalVar::screen->printMapMes("數量: " + std::to_string(globalVar::monsterList.size()));
    for (const auto& m : globalVar::monsterList) {
        int size_ = gapSize_/3*2 - m->getMonstName().size()/3*2;
        int sizeHP = gapHP - getNumberSize(m->getCurHP_m())+2;
        int sizeAP = gapAP - getNumberSize(m->getCurAP_m())+2;
        int sizeDF = gapDF - getNumberSize(m->getCurDF_m())+2;
        globalVar::screen->printMapMes(
            m->getMonstName() + "(" + std::to_string(m->getNumber_m()) +")"+
            std::string(size_, ' ') +"【 HP:" + std::to_string(int(m->getCurHP_m()))+
            std::string(sizeHP, ' ') +"DF:" + std::to_string(int(m->getCurDF_m())) +
            std::string(sizeDF, ' ') + "AP:" + std::to_string(int(m->getCurAP_m())) + std::string(sizeAP, ' ') +"】");
    //     globalVar::screen->printMapMes(
    //         m->getMonstName() + "(" + std::to_string(m->getNumber_m()) + ")"+ std::string(size_, ' ') + "【 HP:" + std::to_string(m->getCurHP_m()) + "\t" +
    //         "DF:" + std::to_string(m->getCurDF_m()) + "\tAP:" + std::to_string(m->getCurAP_m()) + "\t】");
    }

    globalVar::screen->printMapMes(" ");
    return 0;
}

int funcFight(const std::vector<std::string>& tokens) {
    if (tokens.size() != 2) {
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }
    int id;
    try {
        id = std::stoi(tokens[1]);
    } catch (const std::invalid_argument& e) {
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }
    globalVar::screen->printMapMes("戰鬥開始!!!");
    globalVar::ft->getFightWithMonsId(id);

    return 0;
}

int funcThrow(const std::vector<std::string>& tokens) {
    if (tokens.size() != 1) {
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }

    globalVar::screen->printMapMes("Bag List:");
    globalVar::bg->showBagList();
    globalVar::screen->printMapMes(" ");
    if (!globalVar::bg->isBagCanUse())
        return 1;
    
    std::string ins;
    globalVar::screen->setCursorVisible(true);

    globalVar::screen->setInfoCursorPos(0, 5);
    std::getline(std::cin, ins);
    if (ins.size() == 0) {
        globalVar::screen->clearMes(0, 5, 10);
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }
    int index;
    try {
        index = stoi(ins);
    } catch (const std::invalid_argument& e) {
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }
    globalVar::screen->clearMes(0, 5, 10);

    globalVar::screen->setMes("你確定要丟棄這個物品?(y/n)\n", 0, 4);
    globalVar::screen->setInfoCursorPos(0, 5);
    std::getline(std::cin, ins);
    if (ins.size() != 1) {
        globalVar::screen->clearMes(0, 4, 30);
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }
    globalVar::screen->clearMes(0, 4, 30);
    globalVar::screen->clearMes(0, 5, 10);

    globalVar::screen->setCursorVisible(false);
    if (ins == "N" || ins == "n" || ins == "no" || ins == "No") {
        globalVar::screen->printMapMes("取消刪除裝備!!!");
        return 1;
    } else if (ins != "y" && ins != "Y" && ins != "yse" && ins != "Yes") {
        globalVar::screen->printMapMes("輸入錯誤!!!");
        return 1;
    }

    globalVar::bg->throwOut(index);

    return 0;
}

int funcSkillUp(const std::vector<std::string>& tokens) {
    if (tokens.size() != 1) {
        globalVar::screen->clearMes(0, 0, 30);
        printMes("Command error!!!", 0, 0, 4);
        return 1;
    }

    // if (globalVar::screen->getCurCity() != "skillShop")
    return 0;
}

Instruction::Instruction() {
    funcMap["exit"] = reinterpret_cast<void*>(funcExit);
    funcMap["move"] = reinterpret_cast<void*>(funcMove);
    funcMap["save"] = reinterpret_cast<void*>(funcSave);
    funcMap["shop"] = reinterpret_cast<void*>(funcShop);
    funcMap["bag"] = reinterpret_cast<void*>(funcShowBag);
    funcMap["help"] = reinterpret_cast<void*>(funcHelp);
    funcMap["ls"] = reinterpret_cast<void*>(funcMonsList);
    funcMap["fight"] = reinterpret_cast<void*>(funcFight);
    funcMap["skill"] = reinterpret_cast<void*>(funcSkillUp);
    funcMap["throw"] = reinterpret_cast<void*>(funcThrow);
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
        printMes("指令錯誤!!!", 0, 0, 4);
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

void Instruction::printConmand() const {
    for (const auto& s : funcMap)
        globalVar::screen->printMapMes(std::string(8, ' ') + s.first);
}

void printMes(const std::string& mes, SHORT x, SHORT y, int color) {
    globalVar::screen->setColor(color);
    globalVar::screen->setMes(mes, x, y);
    globalVar::screen->setColor();
}

#endif
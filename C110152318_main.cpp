#include <conio.h>
#include <algorithm>
#include <iostream>
#include <nlohmann/json.hpp>
#include <cmath>

#include "libraries/C110152318_MyFunction.h"
using namespace std;

void initializeGame();
inline void genMonster();
BOOL CtrlHandler(DWORD fdwCtrlType);

int main() {
    initializeGame();
    // User user;

    SetConsoleCtrlHandler(CtrlHandler, true);
    while (globalVar::gameState) {
        if (_kbhit()) {
            switch (_getch()) {
                case 119:  // 上
                case 72:
                    globalVar::screen->move(NORTH);
                    break;
                case 115:  // 下
                case 80:
                    globalVar::screen->move(SOUTH);
                    break;
                case 97:  // 左
                case 75:
                    globalVar::screen->move(WEST);
                    break;
                case 100:  // 右
                case 77:
                    globalVar::screen->move(EAST);
                    break;
                case 47:
                    int ti = globalVar::Command->insertCommand();
                    if (ti == -1) globalVar::gameState = false;
                    break;
            }
        }

        globalVar::screen->setCursorVisible(false);
        globalVar::screen->clearAllMap();
        globalVar::screen->displayMap();
        globalVar::user->showInfo();
        genMonster();
        setfps(60);
    }

    globalVar::bg->saveBagItem();
    globalVar::user->saveData();
    system("cls");
    system("pause");
    return 0;
}

void initializeGame() {
    globalVar::Command = new Instruction;

    globalVar::user = new User;
    globalVar::bg = new Bag;
    globalVar::ft = new Fight();
    globalVar::screen->setConsoleName(L"C110152318期末作業⚔⛊");

    // for general monster rand.
    initMonsOccurProb();
    srand(time(nullptr));

    globalVar::user->showInfo();
    // for (int i = 2; i <= 1000; ++i)
    //     globalVar::user->expUp(100);
    if (globalVar::user->getCurId() == "10")
        globalVar::user->setLevel(100);
}

void genMonster() {
    if (!globalVar::genMons) {
        if (globalVar::killNum > 5 || globalVar::monsterList.size() == 0) {
            globalVar::screen->printMapMes("生成怪物:");
            genAnyNumMons(rand()%5+1);
            globalVar::killNum = 0;
        }
        return;
    }
    // clear monsterList
    for (auto m : globalVar::monsterList)
        delete m;
    globalVar::monsterList.clear();
    if (globalVar::screen->getCurCity() == "market") return;
    // todo
    // gen monster
    // if (globalVar::screen->getCurCity() == "DemonCity") {
    //     globalVar::monsterList.emplace_back(new Monster(
    //         ranInfo[pos].name, ranInfo[pos].HP, ranInfo[pos].DF, ranInfo[pos].AP,
    //         MonstData::curMonsNum, ranInfo[pos].EXP));
    // }
    // if (globalVar::screen->getCurCity() == "desertTemple" && globalVar::genMons) {
    // }
    if (globalVar::genMons) {  // 刷新地圖時候
        globalVar::screen->printMapMes("怪物資訊:");
        MonstData::curMonsNum = 0;
        // globalVar::monsterList.resize(MONSIZE, nullptr);
        int n = rand()%MONSIZE+3;
        globalVar::screen->printMapMes("數量: "+to_string(n));
        genAnyNumMons(n);
    }
    globalVar::genMons = false;
}

BOOL CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
        case CTRL_CLOSE_EVENT:
            Beep(750, 100);
            globalVar::bg->saveBagItem();
            globalVar::user->saveData();  // bug
            globalVar::gameState = false;
            return false;
        default:
            return true;
    }
}
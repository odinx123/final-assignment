#include <conio.h>

#include <iostream>
#include <nlohmann/json.hpp>

#include "libraries/C110152318_MyFunction.h"
using namespace std;

inline void initializeGame();

int main() {
    initializeGame();
    // User user;
    bool gameState = true;

    while (gameState) {
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
                    auto ti = globalVar::Command->insertCommand();
                    gameState = ti == -1 ? 0 : 1;
                    break;
            }
        }
        setfps(60);
    }
    
    system("cls");
    system("pause");
    return 0;
}

void initializeGame() {
    globalVar::user = new User;
    globalVar::Command = new Instruction;
    globalVar::bg = new Bag;
    globalVar::screen->setConsoleName(L"C110152318期末作業⚔⛊");

    globalVar::user->showInfo();
    // for (int i = 2; i <= 100000; ++i)
        // globalVar::user->expUp(100);
    // globalVar::user->setLevel(100);

    globalVar::user->saveData();
}
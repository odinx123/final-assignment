#include <conio.h>

#include <codecvt>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

#include "libraries/MyFunction.h"
using namespace std;

inline void initializeGame();

int main() {
    initializeGame();
    // User user;

    while (true) {
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
                    globalVar::Command->insertCommand();
                    break;
            }
        }
        setfps(60);
    }

    system("pause");
    return 0;
}

void initializeGame() {
    User user;
    globalVar::Command = new Instruction;
    globalVar::screen->setConsoleName(L"C110152318期末作業⚔⛊");

    user.wa->showInfo();
    user.wa->addAP(100, 5000);
    // user1.showInfo();
    // user1.addAP(100, 2000);
}
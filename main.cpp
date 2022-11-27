#include <conio.h>

#include <codecvt>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

#include "libraries/MyFunction.h"
using namespace std;
using namespace nlohmann;

// std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
std::stringstream stream;

void command(Map& screen);

int main() {
    Map screen(25);
    screen.setConsoleName(L"C110152318期末作業⚔⛊");
    // stream.imbue(loc);

    while (true) {
        if (_kbhit()) {
            switch (_getch()) {
                case 119:  // 上
                case 72:
                    screen.move(NORTH);
                    break;
                case 115:  // 下
                case 80:
                    screen.move(SOUTH);
                    break;
                case 97:  // 左
                case 75:
                    screen.move(WEST);
                    break;
                case 100:  // 右
                case 77:
                    screen.move(EAST);
                    break;
                case 47:
                    command(screen);
                    break;
            }
        }
        setfps(60);
    }

    system("pause");
    return 0;
}

void command(Map& screen) {
    std::string inpt;
    std::string ins;

    screen.clearMes(0, 0, 30);

    screen.setInfoCursorPos(0, 4);
    std::string mes = "請輸入指令:";
    std::cout << mes << std::endl;
    screen.setCursorVisible(true);

    std::getline(std::cin, inpt);

    stream.clear();
    stream.str(inpt);
    SHORT insSize = stream.str().size();

    stream >> ins;
    if (ins == "move") {
        int i;
        screen.clearMes(0, 5, insSize);
        screen.setInfoCursorPos(0, 5);
        auto orgPos = screen.getCursorPos();
        for (int j = 0; j < screen.getCityNum(); ++j) {
            screen.setInfoCursorPos(30, j+1);
            std::cout << j << ": " << screen.getChsCityName(j);
        }
        screen.setStdCursorPos(orgPos.X, orgPos.Y);

        (cin >> i).get();
        ins = screen.getEngCityName(i);
        screen.loadMap(screen.getEngCityName(i));
    } else {
        screen.setColor(4);
        screen.setMes("指令錯誤!!!", 0, 0);
        screen.setColor();
    }
    screen.setInfoCursorPos(0, 4);
    std::cout << mes.size() << std::endl;
    screen.clearMes(0, 4, mes.size() / 1.5 + 1);
    screen.clearMes(0, 5, insSize);

    for (int j = 0; j < screen.getCityNum(); ++j)
        screen.clearMes(30, j+1, screen.getChsCityName(j).size()+1);

    screen.setCursorVisible(false);
}
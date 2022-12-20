#include <windows.h>
#include <stdlib.h>

#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

//   0 = 黑色      8 = 灰色
//   1 = 蓝色      9 = 淡蓝色
//   2 = 绿色      A = 淡绿色
//   3 = 浅绿色    B = 淡浅绿色
//   4 = 红色      C = 淡红色
//   5 = 紫色      D = 淡紫色
//   6 = 黄色      E = 淡黄色
//   7 = 白色      F = 亮白色
using namespace std;

void PrintXueHua() {
    vector<COORD> snow(30, {0, 0});
    srand((int)time(0));
    CONSOLE_CURSOR_INFO cci{1, false};

    for (int i = 0; i < snow.size(); ++i) {  // init the snow.
        COORD pos;
        pos.X = rand() % 120;
        pos.Y = rand() % 15;
        snow[i] = pos;
    }
    while (1) {
        HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorInfo(hd, &cci);
        COORD pos;
        // 左侧雪花生成
        for (int i = 0; i < snow.size(); ++i) {
            SetConsoleCursorPosition(hd, snow[i]);
            cout << " ";
            if (snow[i].Y - 1 >= 15) {
                snow[i].X = rand() % 120;
                snow[i].Y = rand() % 6;
                SetConsoleCursorPosition(hd, snow[i]);
                cout << "*";
            } else {
                ++snow[i].Y;
                SetConsoleCursorPosition(hd, snow[i]);
                cout << "*";
            }
        }
        
        Sleep(150);
    }
}

int main() {
    system("color 0f");
    // SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // 将雪花颜色更改为白色
    // 调用生成雪花函数
    PrintXueHua();
    system("pause");
    return 0;
}
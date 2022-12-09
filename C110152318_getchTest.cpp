#include <conio.h>
#include <iostream>
#include <windows.h>
// #include "libraries/function.h"
using namespace std;


int main() {
    CONSOLE_CURSOR_INFO cci{1, false};
    auto stdBuf = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    SetConsoleCursorInfo(stdBuf, &cci);

    while (true) {
        if (_kbhit()) {
            cout << _getch() << endl;
        }
        // setfps(60);
    }

    return 0;
}
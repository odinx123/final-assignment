#include <fstream>
#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    string pic;
    ifstream file_in("ani.txt");
    HANDLE stdBuf = GetStdHandle(STD_OUTPUT_HANDLE);
    vector<string> picMatrix;

    int cnt = 0;
    string temp;
    while (!file_in.eof()) {
        pic.clear();
        for (int i = 0; i < 36; ++i) {
            getline(file_in, temp);
            pic += temp;
            pic.push_back('\n');
        }
        picMatrix.emplace_back(pic);
    }

    cnt = 0;
    CONSOLE_CURSOR_INFO cci{1, false};
    while (true) {
        for (int i = 0; i < picMatrix.size(); ++i) {
            SetConsoleCursorInfo(stdBuf, &cci);
            SetConsoleCursorPosition(stdBuf, {0, 0});
            WriteConsole(stdBuf, picMatrix[i].c_str(), picMatrix[i].size(), nullptr, nullptr);
            Sleep(50);
        }
    }

    system("pause");
    return 0;
}
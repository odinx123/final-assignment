#include <windows.h>
#include <iomanip>
#include <iostream>
using namespace std;

auto stdBuf = GetStdHandle(STD_OUTPUT_HANDLE);

void setColor(WORD color = 7) {
    SetConsoleTextAttribute(stdBuf, color);
}

int main() {
    SetConsoleOutputCP(65001);

    string testSentense = "⚔";
    for (int i = 1; i <= 256; ++i) {
        setColor(i-1);
        cout << "<" << setw(3) << i-1;
        cout << testSentense;
        cout << ">";
        if (i % 16 == 0)
            cout << endl;
    }
    cout << endl;
    setColor();

    system("pause");
    return 0;
}
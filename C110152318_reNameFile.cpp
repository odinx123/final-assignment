#include <windows.h>
#include <string>
#include <iostream>

#include "libraries/C110152318_function.h"
using namespace std;

int main() {
    // SetConsoleOutputCP(65001);
    string path;
    vector<string> fileName;
    get_path(path);
    getFiles(path, fileName);

    int cnt = 0;
    for (const auto& i : fileName) {
        auto pos = i.find_last_of('.');
        if (pos != string::npos && (i.substr(pos) == ".png")) {
            auto pos2 = i.find_last_of('t')+6;
            cout << i.substr(0, pos2) << endl;
            cout << i << endl;

            string newName = i.substr(0, pos2) + "img" + to_string(cnt) + ".png";
            rename(i.c_str(), newName.c_str());
            cout << newName << endl;
            ++cnt;
        }
    }

    cout << endl << cnt << endl;

    system("pause");
    return 0;
}
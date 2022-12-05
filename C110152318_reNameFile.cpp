#include <windows.h>

#include <iostream>

#include "libraries/C110152318_function.h"
using namespace std;

int main() {
    SetConsoleOutputCP(65001);
    string path;
    vector<string> fileName;
    get_path(path);
    getFiles(path, fileName);

    int cnt = 0;
    for (const auto& i : fileName) {
        auto pos = i.find_last_of('.');
        if (pos != string::npos && (i.substr(pos) == ".cpp" || i.substr(pos) == ".h")) {
            auto pos2 = i.find_last_of('\\')+1;
            auto s = i.substr(i.find_last_of('\\')+1);
            if (s[0] == 'C' || s[0] == 'c') continue;
            rename(i.c_str(), (i.substr(0, i.size()-s.size())+"C110152318_" + s).c_str());
            cout << i.substr(0, i.size()-s.size())+"C110152318_" + s << endl;
            ++cnt;
        }
    }

    cout << endl << cnt << endl;

    system("pause");
    return 0;
}
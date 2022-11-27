#include <fcntl.h>
#include <io.h>
#include <windows.h>

#include <codecvt>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <vector>

// #include "libraries/MyFunction.h"
// #include "nlohmann/json.hpp"
using namespace std;
// using namespace nlohmann;

// auto stdBuf = GetStdHandle(STD_OUTPUT_HANDLE);
std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
ifstream file_in;
// ofstream file_out;

// class test {
//     friend void to_json(nlohmann::json& j, const test& t);
//     friend void from_json(const nlohmann::json& j, test& t);
//     int a = 0;
//     int b = 1;

//    public:
//     int donothing() { return 1; }
// };

// void to_json(nlohmann::json& j, const test& t) {
//     j = nlohmann::json{{"a", t.a}, {"b", t.b}};
// }
// void from_json(const nlohmann::json& j, test& t) {
//     j.at("a").get_to(t.a);
//     j.at("b").get_to(t.b);
// }

int main() {
    SetConsoleOutputCP(65001);

    string path = "D:\\Programming\\final\\map\\map01.txt";
    file_in.imbue(loc);
    cin.imbue(loc);
    cout.imbue(loc);
    file_in.open(path);
    string s;

    cout << "我" << endl;
    DWORD a;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), L"我", 2, nullptr, nullptr);
    // getline(file_in, s);
    file_in >> s;
    cout << s << endl;
    cin >> s;
    cout << s << endl;

    system("pause");
    return 0;
}
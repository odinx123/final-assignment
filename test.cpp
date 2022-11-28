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

void setColor(WORD color = 7) {
    auto stdBuf = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(stdBuf, color);
}

int main() {
    SetConsoleOutputCP(65001);

    setColor(12);
    for (int i = 0; i < 10; ++i) {
        cout << "♥";
    }
    cout << endl;
    setColor();

    system("pause");
    return 0;
}
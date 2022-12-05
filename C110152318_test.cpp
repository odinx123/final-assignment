#include <windows.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <iomanip>

#include "libraries/C110152318_function.h"
#include "libraries/C110152318_user.h"
using std::chrono::milliseconds;
using std::chrono::seconds;
using namespace std;

class a {
    public:
    int a = 5;
};

class b {
    public:
    int a = 6;
};

int main() {
    unsigned int i = 0xed;
    auto a = i;
    // cout << hex << a << endl;
    i = (i^0x20);
    cout << hex << a << " " << i << endl;
    cout << hex << (i&a) << endl;
    // User user;
    // globalVar::user = new User;
    // cout << getNumberSize(500002);

    // auto a = getClassp(1);

    system("pause");
    return 0;
}
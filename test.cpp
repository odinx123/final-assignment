#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include <atomic>
#include <string>
#include <windows.h>
#include <nlohmann/json.hpp>
#include "libraries/user.h"
#include "libraries/global.h"
using std::chrono::milliseconds;
using std::chrono::seconds;
using namespace std;

int main() {
    User user;
    // globalVar::file_out.open("D:\\Programming\\final\\final-assignment\\player\\test.json");

    system("pause");
    return 0;
}
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

int main() {
    std::string path = "D:\\Programming\\final\\final-assignment";
    ifstream file_in(path+"\\player\\test.json");
    nlohmann::json j;

    file_in >> j;
    cout << j.dump(4) << endl;

    // for (const auto& o : j) {
    //     cout << o << " " << o.value() << endl;
    // }

    system("pause");
    return 0;
}
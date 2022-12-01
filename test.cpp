#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include <atomic>
#include <string>
#include <windows.h>
using std::chrono::milliseconds;
using std::chrono::seconds;
using namespace std;

std::mutex s_mutex;
std::atomic_bool s_running {true};

static void output(std::string const& msg) {
    // std::lock_guard<std::mutex> lk(s_mutex);
    std::this_thread::sleep_for(milliseconds(500));
    std::cout << msg << "\n";
}

void count_thread_func(int d) {
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(seconds(1));
        output("COUNTER AT " + std::to_string(i+1));
    }

    s_running = false;
}

int main() {
    auto start = std::chrono::system_clock::now();
    Sleep(1000);
    cout << (std::chrono::system_clock::now()-start).count()/1000000.f << endl;

    system("pause");
    return 0;
}
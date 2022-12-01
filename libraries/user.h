#ifndef USER_H
#define USER_H 1

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "function.h"
#include "global.h"
#include "job.h"
#include "warrior.h"

#define WARRIOR 1

class User {
   private:
    SHORT number;
    std::string ID;
    std::string passwd;
    std::string path;
    nlohmann::json user;
    std::unordered_map<std::string, std::string> accountList;

   public:
    Warrior* wa = nullptr;

   public:
    User();

    void login();
    void waitAni(const std::string& mes) const;
    void selectJob();
};

User::User() {
    if (globalVar::screen != nullptr)
        delete globalVar::screen;
    globalVar::screen = new Map;

    get_path(path);
    path += "\\player\\user.json";
    globalVar::file_in.open(path);
    globalVar::file_in >> user;
    globalVar::file_in.close();

    for (const auto& j : user["account"])
        accountList[(j.at("ID"))] = j.at("passwd");
    login();
}

void User::login() {
    globalVar::screen->clearMap();

    std::cout << "請輸入帳號: ";
    std::cin >> ID;

    std::string tempCityName, curJob;
    if (accountList.count(ID) > 0) {
        SHORT t = 3;
        while (t) {
            globalVar::screen->setStdCursorPos(0, 1);
            std::cout << std::string(globalVar::screen->getConsoleWidth(), ' ');

            globalVar::screen->setStdCursorPos(0, 1);
            std::cout << "請輸入密碼: ";
            std::cin >> passwd;
            if (accountList.at(ID) == passwd) {
                std::cout << "密碼正確!!!";
                break;
            }
            std::cout << "密碼錯誤，剩下" << --t << "次機會!!!" << std::endl;
        }
        globalVar::screen->clearMap();
        if (t) {
            // todo
            // waitAni("登入中");
        } else {
            std::cout << "登入失敗!!!" << std::endl
                      << "請重設密碼!\n新密碼: ";
            std::cin >> passwd;
            // todo
            // waitAni("已重設密碼，登入中");
        }
        selectJob();
        bool state = false;
        for (const auto& j : user["account"]) {
            if (j.at("ID") == ID) {
                tempCityName = j.at("curCity");
                for (const auto& k : j["job"]) {
                    if (number == k["jobName"]) {
                        if (wa != 0)
                            delete wa;
                        
                        auto status = k["status"];
                        wa = new Warrior(status["HP"], status["DF"], status["AP"]);
                        wa->setCurHP(status["curHP"]);
                        state = true;
                        break;
                    }  // todo
                }
                if (!state) {
                    if (number == WARRIOR) {
                        if (wa != 0)
                            delete wa;
                        wa = new Warrior();
                    }// else if (number == )
                    // todo
                }
                break;
            }
        }
    } else {
        std::cout << "請設定密碼: ";
        std::cin >> passwd;
        selectJob();
        // todo
        // waitAni("創建帳號中");
        tempCityName = "map01";
    }
    globalVar::screen->setAllCity();
    globalVar::screen->loadMap(tempCityName);
    globalVar::screen->showMap(90, 1);
}

void User::waitAni(const std::string& mes) const {
    globalVar::screen->clearMap();
    for (int i = 0; i < 3; ++i) {
        globalVar::screen->setStdCursorPos(0, 0);
        std::cout << std::string(globalVar::screen->getConsoleWidth(), ' ');

        globalVar::screen->setStdCursorPos(0, 0);
        std::cout << mes;
        for (int j = 0; j < 3; ++j) {
            Sleep(200);
            std::cout << '.';
            Sleep(200);
        }
        globalVar::screen->setStdCursorPos(0, 0);
        Sleep(200);
        std::cout << mes;
        Sleep(200);
    }
}

void User::selectJob() {
    // todo
    globalVar::screen->clearMap();
    std::cout << "請選擇職業" << std::endl;
    std::cout << "戰士: 01\n";
    std::cout << "法師: 02\n";
    std::cout << "刺客: 03\n";
    std::cout << "坦克: 04\n";
    std::cin >> number;
    if (number == WARRIOR)
        wa = new Warrior();
}

#endif
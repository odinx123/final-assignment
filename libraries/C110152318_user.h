#ifndef USER_H
#define USER_H 1

#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <cmath>

#include "C110152318_function.h"
#include "C110152318_global.h"
#include "C110152318_jobcomb.h"

#define WARRIOR "1"
#define MAGIC "2"
#define ASSASSIN "3"
#define TANK "4"

#define MAXLEVEL 100

static std::mutex mut;

class User {
   private:
    int level;
    double EXP;
    std::string number;
    std::string ID;
    std::string passwd;
    std::string path;
    nlohmann::json user;
    double needEXP = 10;
    double coin;

   public:
    JobComb* jb = nullptr;

   public:
    User();
    ~User() { delete jb; }

    void login();
    void waitAni(const std::string& mes) const;
    void selectJob();
    void saveData();
    void changeJob();
    void expUp(double l);
    void showInfo(SHORT x = 27, SHORT y = 1) const;
    inline double LevelNeedEXP(int l) const;
    // just up level.
    inline void setLevel(int l);

};

void expUpAni2(double l);

User::User() {
    if (globalVar::screen != nullptr)
        delete globalVar::screen;
    globalVar::screen = new Map;

    get_path(path);
    path += "\\player\\user.json";
    globalVar::file_in.open(path);
    globalVar::file_in >> user;
    globalVar::file_in.close();

    login();
    saveData();
    coin = 100;
}

void User::login() {
    globalVar::screen->clearMap();
    globalVar::screen->setCursorVisible(true);

    std::cout << "請輸入帳號: ";
    (std::cin >> ID).get();

    std::string tempCityName;
    if (user["account"].count(ID) > 0) {
        int t = 3;
        while (t) {
            globalVar::screen->setStdCursorPos(0, 1);
            std::cout << std::string(globalVar::screen->getConsoleWidth(), ' ');

            globalVar::screen->setStdCursorPos(0, 1);
            std::cout << "請輸入密碼: ";
            (std::cin >> passwd).get();
            if (user["account"].at(ID).at("passwd") == passwd) {
                std::cout << "密碼正確!!!";
                break;
            }
            std::cout << "密碼錯誤，剩下" << --t << "次機會!!!" << std::endl;
        }
        tempCityName = user["account"][ID]["curCity"];
        if (t) {
            // todo
            // waitAni("登入中");
        } else {
            globalVar::screen->clearMap();
            std::cout << "登入失敗!!!\n請重設密碼!\n新密碼: ";
            (std::cin >> passwd).get();
            // todo
            // waitAni("已重設密碼，登入中");
        }
        changeJob();
    } else {
        std::cout << "請設定密碼: ";
        (std::cin >> passwd).get();
        tempCityName = "map01";  // todo

        changeJob();

        // todo
        // waitAni("創建帳號中");
    }
    globalVar::screen->setAllCity();
    globalVar::screen->loadMap(tempCityName);
    globalVar::screen->showMap(90, 1);
    globalVar::screen->setCursorVisible(false);
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
    std::cout << "戰士: 01" << std::endl;
    std::cout << "法師: 02" << std::endl;
    std::cout << "刺客: 03" << std::endl;
    std::cout << "坦克: 04" << std::endl;
    std::cout << "輸入編號: ";
    (std::cin >> number).get();

    auto i = std::stoi(number);
    if (i <= 0 || i > 4)
        selectJob();
}

// todo
void User::saveData() {
    auto &userID = user["account"][ID];
    userID["curCity"] = globalVar::screen->getCurCity();
    userID["passwd"] = passwd;

    userID["job"][number] = nlohmann::json{
        {"EXP", EXP},
        {"level", level},
        {"needEXP", needEXP},
        {"status", nlohmann::json{
            {"HP", jb->getHP()},
            {"DF", jb->getDF()},
            {"AP", jb->getAP()},
            {"curHP", jb->getCurHP()}
        }}
    };
    globalVar::file_out.open(path);
    globalVar::file_out << user.dump(4) << std::endl;
    globalVar::file_out.close();
}

void User::changeJob() {
    selectJob();
    if (jb) delete jb;

    auto jobList = user["account"][ID]["job"];
    if (jobList.count(number) > 0) {
        EXP = jobList[number]["EXP"];
        level = jobList[number]["level"];

        jb = new JobComb(
            jobList[number]["status"]["HP"],
            jobList[number]["status"]["DF"],
            jobList[number]["status"]["AP"],
            std::stoi(number)-1
        );
        needEXP = jobList[number]["needEXP"];
        jb->setCurHP(jobList[number]["status"]["curHP"]);
    } else {
        EXP = 0;
        level = 1;
        if (number == WARRIOR) {
            jb = new JobComb(20, 20, 15, std::stoi(WARRIOR)-1);
        } else if (number == MAGIC) {
            jb = new JobComb(16, 8, 30, std::stoi(MAGIC)-1);
        } else if (number == ASSASSIN) {
            jb = new JobComb(10, 12, 25, std::stoi(ASSASSIN)-1);
        } else if (number == TANK) {
            jb = new JobComb(32, 23, 10, std::stoi(TANK)-1);
        }
    }
}

void User::expUp(double l) {
    EXP += l;
    while (level < MAXLEVEL && (fabs(needEXP-EXP) <= 0.0001 || EXP >= needEXP)) {
        ++level;
        EXP -= needEXP;
        jb->setLevelStatus();
        if (level < MAXLEVEL)
            needEXP += (needEXP*0.1+3000) * 0.2 + 10;
    }  // 0.02^(n-1)*(needEXP+(610/0.02))-(610/0.02)
    if (level >= MAXLEVEL)
        EXP = needEXP;
    showInfo();
}

void User::showInfo(SHORT x, SHORT y) const {
    globalVar::screen->setColor(10);
    if (number == WARRIOR)
        globalVar::screen->setMes("戰士 Lv"+std::to_string(level), x, y);
    else if (number == MAGIC)
        globalVar::screen->setMes("法師 Lv"+std::to_string(level), x, y);
    else if (number == ASSASSIN)
        globalVar::screen->setMes("刺客 Lv"+std::to_string(level), x, y);
    else if (number == TANK)
        globalVar::screen->setMes("坦克 Lv"+std::to_string(level), x, y);
    globalVar::screen->setColor(14);
    globalVar::screen->setMes("經驗:", x, y+1);
    
    globalVar::screen->setColor(224);
    std::string tempS = std::to_string(int(EXP))+"/"+std::to_string(int(needEXP));

    double n = double(EXP)/needEXP;
    int size_ = getNumberSize(LevelNeedEXP(100))*2 + 3;
    int start = (size_ - tempS.size()) / 2;
    for (int i = 0; i < size_; ++i) {
        if (double(i+1)/(size_) > n)
            globalVar::screen->setColor(128);
        if (i >= start && i-start < tempS.size())
            globalVar::screen->setMes(std::string(1, tempS.at(i-start)), x+i+6, y+1);
        else
            globalVar::screen->setMes(" ", x+i+6, y+1);
    }
    jb->showAllInfo(x, y+1);
}

double User::LevelNeedEXP(int l) const {
    return pow(1.02, l-2)*30510-30500;
}

void User::setLevel(int l) {
    for (int i = level+1; i <= l; ++i)
        globalVar::user->expUp(globalVar::user->LevelNeedEXP(i)-EXP);
}

#endif
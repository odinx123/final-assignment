#ifndef USER_H
#define USER_H 1

#include <cmath>
#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>

#include "C110152318_function.h"
#include "C110152318_global.h"
#include "C110152318_jobcomb.h"

#define WARRIOR "1"
#define MAGIC "2"
#define ASSASSIN "3"
#define TANK "4"

#define MAXLEVEL 100
#define MAXSKLEVEL 6
#define BASEATRHP 10
#define BASEATRDF 0.1
#define BASEATRAP 5

static std::mutex mut;

class User {
   private:
    int level;
    double EXP;
    std::string number;
    std::string ID;
    std::string passwd;
    std::string path;
    double needEXP = 10;
    double coin;
    bool live = true;
    int skLevel = 1;
    double CRT_Rate = 0.1;
    double CRT_dmg = 0.5;

   public:
    JobComb* jb = nullptr;
    int totleKill = 0;
    int skillPoint = 0;
    int skillUpPoint = 0;

    double atrHP = 0;
    double atrDF = 0;
    double atrAP = 0;
    int atrPoint = 0;

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
    void showEXP(SHORT x = 27, SHORT y = 1) const;
    inline void showCoin(SHORT x = 27, SHORT y = 3) const;
    inline double LevelNeedEXP(int l) const;
    // just up level.
    inline void setLevel(int l);
    double getCoin() const { return coin; }
    void changeCoin(double m) { coin += m; }
    std::string getCurId() const { return ID; }
    std::string getCurJob() const { return number; }
    bool isLive() const { return live; }
    void setLive(bool s) { live = s; }
    int useSkill();
    int incSklv() { return (skLevel += skLevel + 1 > MAXSKLEVEL ? 0 : 1); }
    int getSklevel() const { return skLevel; }
    int getLevel() const { return level; }
    double getCritiCalRate() const { return CRT_Rate; }
    void incCritiCalRate(double rate) { CRT_Rate += rate; }
    void decCritiCalRate(double rate) { CRT_Rate -= rate; }
    double getCritiCalDmg() { return CRT_dmg; }
    void incCritiCalDmg(double dmg) { CRT_dmg += dmg; }
    void decCritiCalDmg(double dmg) { CRT_dmg -= dmg; }
    void incSkillPoint() { skillPoint += (((skillPoint + 1) <= level) ? 1 : 0); }
    void minSkillPoint() { skillPoint -= skLevel*5; }
    bool isSkillFull() const { return skillPoint >= level; }
    std::string getSkillName() const;
    std::string getSkillEffect() const;
};

User::User() {
    if (globalVar::screen != nullptr)
        delete globalVar::screen;
    globalVar::screen = new Map;

    get_path(path);
    path += "\\player\\user.json";
    globalVar::file_in.open(path);
    globalVar::file_in >> globalVar::jin;
    globalVar::file_in.close();

    login();

    // 爆擊率、爆傷
    if (number == WARRIOR) {
        CRT_Rate = 0.2;
        CRT_dmg = 0.75 * (level / 5.0);
    } else if (number == MAGIC) {
        CRT_Rate = 0.15;
        CRT_dmg = 833 * (level / 5.0);
    } else if (number == ASSASSIN) {
        CRT_Rate = 0.3;
        CRT_dmg = 1.45 * (level / 5.0);
    } else if (number == TANK) {
        CRT_Rate = 0.1;
        CRT_dmg = 0.54 * (level / 5.0);
    }
    saveData();
}

void User::login() {
    // globalVar::screen->clearMap();
    globalVar::screen->setCursorVisible(true);

    std::cout << "請選擇登入或刪除帳號(0/1): ";
    int inpt;
    std::string inidx;
    getline(std::cin, inidx);
    try {
        inpt = std::stoi(inidx);
    } catch (const std::invalid_argument& e) {
        system("cls");
        std::cout << "輸入錯誤!!!\n";
        return login();
    }
    if (inpt) {
        for (const auto& j : globalVar::jin["account"].items())
            std::cout << j.key() << std::endl;
        std::cout << "輸入要刪除的帳號: " << std::endl;
        getline(std::cin, inidx);

        if (globalVar::jin["account"].count(inidx) > 0) {
            system("cls"); 
            std::cout << "刪除帳號: " + inidx + "                  " << std::endl;
            globalVar::jin["account"].erase(inidx);
            globalVar::file_out.open(path);
            globalVar::file_out << globalVar::jin.dump(4) << std::endl;
            globalVar::file_out.close();
        } else {
            system("cls");
            std::cout << "沒有這個帳號!!!                  " << std::endl;
        }
        return login();
    }

    system("cls");
    std::cout << "請輸入帳號: ";
    (std::cin >> ID).get();

    std::string tempCityName;
    if (globalVar::jin["account"].count(ID) > 0) {
        int t = 3;
        totleKill = globalVar::jin["account"][ID]["totleKill"];
        while (t) {
            globalVar::screen->setStdCursorPos(0, 1);
            std::cout << std::string(globalVar::screen->getConsoleWidth(), ' ');

            globalVar::screen->setStdCursorPos(0, 1);
            std::cout << "請輸入密碼: ";
            (std::cin >> passwd).get();
            if (globalVar::jin["account"].at(ID).at("passwd") == passwd) {
                std::cout << "密碼正確!!!";
                break;
            }
            std::cout << "密碼錯誤，剩下" << --t << "次機會!!!" << std::endl;
        }
        tempCityName = globalVar::jin["account"][ID]["curCity"];
        if (t) {
            // todo
            // waitAni("登入中");
        } else {
            // globalVar::screen->clearMap();
            system("cls");
            std::cout << "登入失敗!!!\n請重設密碼!\n新密碼: ";
            t = 2;
            std::string prePasswd;
            auto curPos = globalVar::screen->getCursorPos();
            while (t--) {
                globalVar::screen->setStdCursorPos(curPos.X, curPos.Y);
                prePasswd = passwd;
                (std::cin >> passwd).get();
                globalVar::screen->setStdCursorPos(8, 2);
                std::cout << "                                         ";
            }
            if (passwd == prePasswd) {
                // todo
                // waitAni("已重設密碼，登入中");
            } else {
                system("cls");
                return login();
            }
        }
        changeJob();
        coin = globalVar::jin["account"][ID]["money"];
    } else if (globalVar::jin["account"].size() >= 3) {
        system("cls");
        std::cout << "帳號數量已達限制!!!                " << std::endl;
        return login();
    } else {
        std::cout << "請設定密碼(新帳號): ";
        (std::cin >> passwd).get();
        tempCityName = "lobby";  // todo

        changeJob();

        // todo
        // waitAni("創建帳號中");
        coin = 100;
    }
    system("cls");
    globalVar::screen->setAllCity();
    globalVar::screen->loadMap(tempCityName);
    globalVar::screen->showMap(90, 1);
    globalVar::screen->setCursorVisible(false);

    // 超級帳號
    if (ID == "10") {
        if (level < 100)
            setLevel(100);
        if (coin <= 10000000)
            changeCoin(10000000);
    }
    // 超級帳號

    if (globalVar::jin["account"][ID]["job"][number]["live"] == false) {
        live = true;
        jb->healHP(1);
        globalVar::screen->printMapMes("【 已經復活 】");
        globalVar::screen->printMapMes(" ");
    }
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
    // globalVar::screen->clearMap();
    system("cls");
    std::cout << "請選擇職業" << std::endl;
    std::cout << "戰士: 01" << std::endl;
    std::cout << "法師: 02" << std::endl;
    std::cout << "刺客: 03" << std::endl;
    std::cout << "坦克: 04" << std::endl;
    std::cout << "輸入編號: ";
    (std::cin >> number).get();

    int i;
    try {
        i = std::stoi(number);
        if (i <= 0 || i > 4)
            selectJob();
    } catch (const std::invalid_argument& e) {
        selectJob();
    }
}

// todo
void User::saveData() {
    auto& userID = globalVar::jin["account"][ID];
    userID["curCity"] = globalVar::screen->getCurCity();
    userID["passwd"] = passwd;
    userID["money"] = coin;
    userID["totleKill"] = totleKill;
    userID["job"][number] = nlohmann::json{
        {"skillPoint", skillPoint},
        {"skillUpPoint", skillUpPoint},
        {"live", live},
        {"EXP", EXP},
        {"level", level},
        {"needEXP", needEXP},
        {"skillLevel", skLevel},
        {"atrPoint", atrPoint},
        {"atrHP", atrHP},
        {"atrDF", atrDF},
        {"atrAP", atrAP},
        {"status", nlohmann::json{
                       {"HP", jb->getHP()},
                       {"DF", jb->getDF()},
                       {"AP", jb->getAP()},
                       {"curHP", jb->getCurHP()}}}};

    // globalVar::file_out.open(path);
    // globalVar::file_out << globalVar::jin.dump(4) << std::endl;
    // globalVar::file_out.close();
}

void User::changeJob() {
    selectJob();
    if (jb) delete jb;

    auto jobList = globalVar::jin["account"][ID]["job"];
    if (jobList.count(number) > 0) {
        EXP = jobList[number]["EXP"];
        level = jobList[number]["level"];
        skLevel = jobList[number]["skillLevel"];

        jb = new JobComb(
            jobList[number]["status"]["HP"],
            jobList[number]["status"]["DF"],
            jobList[number]["status"]["AP"],
            std::stoi(number) - 1);
        needEXP = jobList[number]["needEXP"];
        jb->setCurHP(jobList[number]["status"]["curHP"]);
        skillPoint = jobList[number]["skillPoint"];
        skillUpPoint = jobList[number]["skillUpPoint"];
        atrPoint = jobList[number]["atrPoint"];
        atrAP = jobList[number]["atrAP"];
        atrDF = jobList[number]["atrDF"];
        atrHP = jobList[number]["atrHP"];
    } else {
        EXP = 0;
        level = 1;
        if (number == WARRIOR) {
            jb = new JobComb(20, 20, 15, std::stoi(WARRIOR) - 1);
        } else if (number == MAGIC) {
            jb = new JobComb(16, 8, 30, std::stoi(MAGIC) - 1);
        } else if (number == ASSASSIN) {
            jb = new JobComb(10, 12, 25, std::stoi(ASSASSIN) - 1);
        } else if (number == TANK) {
            jb = new JobComb(32, 23, 10, std::stoi(TANK) - 1);
        }
    }
}

void User::expUp(double l) {
    EXP += l;
    while (level < MAXLEVEL && (fabs(needEXP - EXP) <= 0.0001 || EXP >= needEXP)) {
        ++skillUpPoint;  // 加上技能升級點數
        ++atrPoint;  // 加上屬性點數
        skillPoint = (++level);  // 加上技能點數上限(可以釋放技能)
        EXP -= needEXP;
        jb->setLevelStatus();
        if (level < MAXLEVEL)
            needEXP += (needEXP * 0.1 + 3000) * 0.2 + 10;
        // 設定爆擊傷害和機率
        if (number == WARRIOR) {
            CRT_Rate = 0.2;
            CRT_dmg = 0.75 * (level / 5.0);
        } else if (number == MAGIC) {
            CRT_Rate = 0.15;
            CRT_dmg = 833 * (level / 5.0);
        } else if (number == ASSASSIN) {
            CRT_Rate = 0.3;
            CRT_dmg = 1.45 * (level / 5.0);
        } else if (number == TANK) {
            CRT_Rate = 0.1;
            CRT_dmg = 0.54 * (level / 5.0);
        }
        if (globalVar::set) {  // 套裝模式
            CRT_Rate += 0.2;
        }
    }  // 0.02^(n-1)*(needEXP+(610/0.02))-(610/0.02)
    if (level >= MAXLEVEL)
        EXP = needEXP;
    showInfo();
}

void User::showInfo(SHORT x, SHORT y) const {
    showEXP();
    showCoin();

    jb->showAllInfo(x, y + 3);
}

double User::LevelNeedEXP(int l) const {
    return pow(1.02, l - 2) * 30510 - 30500;
}

void User::setLevel(int l) {
    for (int i = level + 1; i <= l; ++i)
        expUp(LevelNeedEXP(i) - EXP);
}

void User::showCoin(SHORT x, SHORT y) const {
    // globalVar::screen->clearMes(x, y-1, 10);
    globalVar::screen->setMes(
        "金錢: $" + std::to_string(int(coin)) + std::string(7, ' '), x, y - 1);
}

void User::showEXP(SHORT x, SHORT y) const {
    globalVar::screen->setColor(10);
    if (number == WARRIOR)
        globalVar::screen->setMes(ID+" 【戰士】 Lv" + std::to_string(level), x, y);
    else if (number == MAGIC)
        globalVar::screen->setMes(ID+" 【法師】 Lv" + std::to_string(level), x, y);
    else if (number == ASSASSIN)
        globalVar::screen->setMes(ID+" 【刺客】 Lv" + std::to_string(level), x, y);
    else if (number == TANK)
        globalVar::screen->setMes(ID+" 【坦克】 Lv" + std::to_string(level), x, y);

    globalVar::screen->setColor(14);
    globalVar::screen->setMes("經驗: ", x, y + 3);
    globalVar::screen->setColor(224);
    std::string tempS = std::to_string(int(EXP)) + "/" + std::to_string(int(needEXP));

    double n = double(EXP) / needEXP;
    int size_ = getNumberSize(LevelNeedEXP(100)) * 2 + 3;
    int start = (size_ - tempS.size()) / 2;

    for (int i = 0; i < size_; ++i) {
        if (double(i + 1) / (size_) > n)
            globalVar::screen->setColor(128);
        if (i >= start && i - start < tempS.size())
            globalVar::screen->setMes(std::string(1, tempS.at(i - start)), x + i + 6, y + 3);
        else
            globalVar::screen->setMes(" ", x + i + 6, y + 3);
    }
    globalVar::screen->setColor();
    globalVar::screen->setMes(
        "擊殺: " + std::to_string(totleKill) + " 技能點: " + std::to_string(skillPoint) + "   ", x, y + 2);
}

int User::useSkill() {
    if (skillPoint - skLevel < 0) return -1;
    minSkillPoint();
    globalVar::screen->printMapMes("消耗" + std::to_string(skLevel*5) + "點技能點");
    if (number == WARRIOR) {
        globalVar::screen->printMapMes(
            "【戰士】發動了技能【" + std::to_string(4 * skLevel) + "連斬】,連續攻擊 " +
            std::to_string(4 * skLevel) + " 次, LV: " + std::to_string(skLevel));
        return 0;
    } else if (number == MAGIC) {
        globalVar::screen->printMapMes(
            "【法師】發動了技能【祝福】,恢復了 " + std::to_string(10 * skLevel) +
            "% 生命, LV: " + std::to_string(skLevel));
        jb->healHP(jb->getHP() * (10.0 * skLevel / 100.0));
        return 1;
    } else if (number == ASSASSIN) {
        globalVar::screen->printMapMes(
            "【刺客】發動了技能【吸血】,吸收攻擊 " + std::to_string(10 * skLevel) +
            "% 持續" + std::to_string(5 * skLevel) + "回合, LV: " + std::to_string(skLevel));
        return 2;
    } else if (number == TANK) {
        globalVar::screen->printMapMes(
            "【坦克】發動了技能【榮譽護盾】,防禦增加 " + std::to_string(10 * skLevel) +
            "% 10秒, LV: " + std::to_string(skLevel));
        jb->addDF(10, 10000);
        return 3;
    }
    return -2;
}

std::string User::getSkillName() const {
    if (number == WARRIOR) {
        return "【" + std::to_string(4 * skLevel) + "連斬】";
    } else if (number == MAGIC) {
        return "【祝福】";
    } else if (number == ASSASSIN) {
        return "【吸血】";
    } else if (number == TANK) {
        return "【榮譽護盾】";
    }
    return "";
}

std::string User::getSkillEffect() const {
    if (number == WARRIOR) {
        return "連續攻擊" + std::to_string(4 * skLevel) + "次";
    } else if (number == MAGIC) {
        return "恢復血量" + std::to_string(10 * skLevel) +
               "% -> " + std::to_string(jb->getHP() * (10.0 * skLevel / 100.0)) + "點生命";
    } else if (number == ASSASSIN) {
        return "吸取攻擊的" + std::to_string(10 * skLevel) + "% 持續" + std::to_string(5 * skLevel) + "回合";
    } else if (number == TANK) {
        return "防禦增加" + std::to_string(10 * skLevel) + "% 10秒";
    }
    return "";
}

#endif
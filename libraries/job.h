#ifndef JOB_H
#define JOB_H 1

#include <atomic>
#include <mutex>
#include <thread>

#include "global.h"
#include "map.h"

#define MAXATK 100
#define MAXDEF 100
#define MAXBLOOD 10000

static std::mutex s_mutex, mud1, mud2, mua1, mua2;

class Job {
   private:
    int HP;  // 血量
    int AP;  // 傷害
    int DF;  // 防禦
    int curHP;
    int curAP;
    int curDF;
    int infoX;
    int infoY;
    std::atomic<bool> sdf1, sdf2, sap1, sap2;
    std::atomic<bool> df1, df2, ap1, ap2;

   public:
    Job(int hp, int df, int ap);

    inline void upDef(int df);
    inline void upHP(int hp);
    inline void upAp(int ap);
    inline void healHP(int heal);
    bool deHP(int atk);
    inline void addDF(int df, int ms);
    inline void subDF(int df, int ms);
    inline void addAP(int ap, int ms);
    inline void subAP(int ap, int ms);
    inline void showHP();
    inline void showDF();
    inline void showAP();
    inline void setInfoPos(int x, int y);
    inline void clearAllStatus();
    inline void clearSubDF();
    inline void clearSubAP();
    inline void clearAddDF();
    inline void clearAddAP();
    int getHP() const { return HP; }
    int getDF() const { return DF; }
    int getAP() const { return AP; }
    int getCurHP() const { return curHP; }
    int getCurAP() const { return curAP; }
    int getCurDF() const { return curDF; }
    void setCurHP(SHORT curhp) { curHP = curhp; }
    void setCurDF(SHORT curdf) { curDF = curdf; }
    void setCurAP(SHORT curap) { curAP = curap; }
    void to_json(nlohmann::json& j);

   protected:
    void showAllInfo(SHORT x = 27, SHORT y = 1);

   private:
    void conAddDF(int df, int ms);
    void conSubDF(int df, int ms);
    void conAddAP(int ap, int ms);
    void conSubAP(int ap, int ms);
    void showbloodImg();
    void showDefImg();
    void showAtkImg();
};

Job::Job(int hp, int df, int ap) {
    curHP = HP = hp;
    curDF = DF = df;
    curAP = AP = ap;
    sdf1 = sdf2 = sap1 = sap2 = true;
}

void Job::healHP(int heal) {
    curHP = (curHP + heal > HP) ? HP : curHP + heal;
}

bool Job::deHP(int atk) {
    atk = round(atk * (1 - (double)curDF / MAXDEF));
    if (curHP - atk <= 0) {
        curHP = 0;
        return false;
    }
    curHP -= atk;
    return true;
}

void Job::addDF(int df, int ms) {
    std::thread t(&Job::conAddDF, this, df, ms);
    t.detach();
}

void Job::subDF(int df, int ms) {
    std::thread t(&Job::conSubDF, this, df, ms);
    t.detach();
}

void Job::conAddDF(int df, int ms) {
    df1 = true;
    std::lock_guard<std::mutex> lk(mud1);
    int gap;
    if (curDF + df > MAXDEF) {
        gap = MAXDEF - curDF;
        curDF = MAXDEF;
    } else {
        gap = df;
        curDF += df;
    }
    showAllInfo();
    auto start = std::chrono::system_clock::now();
    while (sdf1 && ((std::chrono::system_clock::now() - start).count() / 1000000.f < ms))
        std::this_thread::yield();
    curDF -= gap;
    showAllInfo();
    df1 = false;
}

void Job::conSubDF(int df, int ms) {
    df2 = true;
    std::lock_guard<std::mutex> lk(mud2);
    int gap;
    if (curDF - df < 0) {
        gap = curDF;
        curDF = 0;
    } else {
        gap = df;
        curDF -= df;
    }
    showAllInfo();
    auto start = std::chrono::system_clock::now();
    while (sdf2 && ((std::chrono::system_clock::now() - start).count() / 1000000.f < ms))
        std::this_thread::yield();
    curDF += gap;
    showAllInfo();
    df2 = false;
}

void Job::addAP(int ap, int ms) {
    std::thread t(&Job::conAddAP, this, ap, ms);
    t.detach();
}

void Job::subAP(int ap, int ms) {
    std::thread t(&Job::conSubAP, this, ap, ms);
    t.detach();
}

void Job::conAddAP(int ap, int ms) {
    ap1 = true;
    std::lock_guard<std::mutex> lk(mua1);
    int gap;
    if (curAP + ap > MAXATK) {
        gap = MAXATK - curAP;
        curAP = MAXATK;
    } else {
        gap = ap;
        curAP += ap;
    }
    showAllInfo();
    auto start = std::chrono::system_clock::now();
    while (sap1 && ((std::chrono::system_clock::now() - start).count() / 1000000.f < ms))
        std::this_thread::yield();
    curAP -= gap;
    showAllInfo();
    ap1 = false;
}

void Job::conSubAP(int ap, int ms) {
    ap2 = true;
    std::lock_guard<std::mutex> lk(mua2);
    int gap;
    if (curAP - ap < 0) {
        gap = curAP;
        curAP = 0;
    } else {
        gap = ap;
        curAP -= ap;
    }
    showAllInfo();
    auto start = std::chrono::system_clock::now();
    while (sap2 && ((std::chrono::system_clock::now() - start).count() / 1000000.f < ms))
        std::this_thread::yield();
    curAP += gap;
    showAllInfo();
    ap2 = false;
}

void Job::upDef(int df) {
    curDF = DF += df;
    showAllInfo();
}

void Job::upHP(int hp) {
    curHP = HP += hp;
    showAllInfo();
}

void Job::upAp(int ap) {
    curAP = AP += ap;
    showAllInfo();
}

void Job::showHP() {
    globalVar::screen->clearMes(infoX, infoY, 20);
    globalVar::screen->setMes("血量: " + std::to_string(curHP) + '/' + std::to_string(HP), infoX, infoY + 1);
}

void Job::showDF() {
    globalVar::screen->clearMes(infoX, infoY + 2, 20);
    globalVar::screen->setMes("防禦: " + std::to_string(curDF) + " %", infoX, infoY + 2);
}

void Job::showAP() {
    globalVar::screen->clearMes(infoX, infoY + 3, 20);
    globalVar::screen->setMes("攻擊: " + std::to_string(curAP), infoX, infoY + 3);
}

void Job::setInfoPos(int x, int y) {
    infoX = x;
    infoY = y;
}

void Job::showbloodImg() {
    int n = int(static_cast<double>(curHP) / HP * 10) % 11;
    globalVar::screen->setInfoCursorPos(infoX, infoY + 5);
    globalVar::screen->setColor(4);
    for (int i = 0; i < n; ++i)
        std::cout << "♥";
    globalVar::screen->setColor(8);
    for (int i = n; i < 10; ++i)
        std::cout << "♥";
    globalVar::screen->setColor();
}

void Job::showDefImg() {
    int n = 0;
    double t = static_cast<double>(curDF) / MAXDEF * 10;
    if (t < 1 && t > 0)
        n = 1;
    else if (t >= 1)
        n = (t > 10) ? 10 : t;
    globalVar::screen->setInfoCursorPos(infoX, infoY + 6);
    globalVar::screen->setColor(3);
    for (int i = 0; i < n; ++i)
        std::cout << "⛨";
    globalVar::screen->setColor(8);
    for (int i = n; i < 10; ++i)
        std::cout << "⛊";
    globalVar::screen->setColor();
}

void Job::showAtkImg() {
    int n = 0;
    double t = static_cast<double>(curAP) / MAXATK * 10;
    if (t < 1 && t > 0)
        n = 1;
    else if (t >= 1)
        n = (t > 10) ? 10 : t;
    globalVar::screen->setInfoCursorPos(infoX, infoY + 7);
    globalVar::screen->setColor(6);
    for (int i = 0; i < n; ++i)
        std::cout << "⚔ ";
    globalVar::screen->setColor(8);
    for (int i = n; i < 10; ++i)
        std::cout << "⚔ ";
    globalVar::screen->setColor();
}

void Job::showAllInfo(SHORT x, SHORT y) {
    std::lock_guard<std::mutex> lk(s_mutex);
    globalVar::screen->setColor();
    setInfoPos(x, y + 1);
    showHP();
    showAP();
    showDF();
    showbloodImg();
    showDefImg();
    showAtkImg();
}

void Job::clearAllStatus() {
    clearSubAP();
    clearAddAP();
    clearSubDF();
    clearAddDF();
}

void Job::clearSubDF() {
    sdf2 = false;
    Sleep(10);
    while (df2)
        ;
    sdf2 = true;
}

void Job::clearSubAP() {
    sap2 = false;
    Sleep(10);
    while (ap2)
        ;
    sap2 = true;
}

void Job::clearAddDF() {
    sdf1 = false;
    Sleep(10);
    while (df1)
        ;
    sdf1 = true;
}

void Job::clearAddAP() {
    sap1 = false;
    Sleep(10);
    while (ap1)
        ;
    sap1 = true;
}

void Job::to_json(nlohmann::json& j) {
    j = nlohmann::json{
        {"HP", HP},
        {"DF", DF},
        {"AP", AP},
    };
}

#endif
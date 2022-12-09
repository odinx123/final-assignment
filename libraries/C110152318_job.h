#ifndef JOB_H
#define JOB_H 1

#include <atomic>
#include <mutex>
#include <thread>

#include "C110152318_global.h"
#include "C110152318_map.h"

#define MAXATK 100000
#define SHOWATK 1500
#define MAXDEF 100

static std::mutex s_mutex, mud1, mud2, mua1, mua2;

class Job {
   private:
    double HP;  // 血量
    double AP;  // 傷害
    double DF;  // 防禦
    double curHP;
    double curAP;
    double curDF;
    int infoX;
    int infoY;
    std::atomic<bool> sdf1, sdf2, sap1, sap2;
    std::atomic<bool> df1, df2, ap1, ap2;

   public:
    Job(double hp, double df, double ap);

    inline void healHP(double heal);
    // return false if die.
    double deHP(double atk);
    inline void addDF(double df, int ms);
    inline void subDF(double df, int ms);
    inline void addAP(double ap, int ms);
    inline void subAP(double ap, int ms);
    double addDF(double df);
    double subDF(double df);
    double addAP(double ap);
    double subAP(double ap);
    double subHP(double hp);
    double addHP(double hp);
    // todo
    inline void chCurDF(double df);
    inline void chCurAP(double ap);
    inline void chCurHP(double hp);
    // todo
    inline void showHP();
    inline void showDF();
    inline void showAP();
    inline void setInfoPos(int x, int y);
    inline void clearAllStatus();
    inline void clearSubDF();
    inline void clearSubAP();
    inline void clearAddDF();
    inline void clearAddAP();
    double getHP() const { return HP; }
    double getDF() const { return DF; }
    double getAP() const { return AP; }
    double getCurHP() const { return curHP; }
    double getCurAP() const { return curAP; }
    double getCurDF() const { return curDF; }
    void setCurHP(double curhp) { curHP = curhp; }
    void setCurDF(double curdf) { curDF = curdf; }
    void setCurAP(double curap) { curAP = curap; }
    void showAllInfo(SHORT x = 27, SHORT y = 3);

   private:
    void conAddDF(double df, int ms);
    void conSubDF(double df, int ms);
    void conAddAP(double ap, int ms);
    void conSubAP(double ap, int ms);
    void showbloodImg();
    void showDefImg();
    void showAtkImg();
};

Job::Job(double hp, double df, double ap) {
    curHP = HP = hp;
    curDF = DF = df;
    curAP = AP = ap;
    df1 = df2 = ap1 = ap2 = false;
    sdf1 = sdf2 = sap1 = sap2 = true;
}

void Job::healHP(double heal) {
    curHP = (curHP + heal > HP) ? HP : curHP + heal;
}

double Job::deHP(double atk) {
    atk = round(atk * (1 - (double)curDF / MAXDEF));
    curHP -= atk;
    if (curHP < 0)
        curHP = 0;
    return atk;
}

void Job::addDF(double df, int ms) {
    std::thread t(&Job::conAddDF, this, df, ms);
    t.detach();
}

void Job::subDF(double df, int ms) {
    std::thread t(&Job::conSubDF, this, df, ms);
    t.detach();
}

void Job::conAddDF(double df, int ms) {
    df1 = true;
    // std::lock_guard<std::mutex> lk(mud1);
    int gap;
    if (curDF + df > MAXDEF) {
        gap = MAXDEF - curDF;
        curDF = MAXDEF;
    } else {
        gap = df;
        curDF += df;
    }
    // showAllInfo();
    auto start = std::chrono::system_clock::now();
    while (sdf1 && ((std::chrono::system_clock::now() - start).count() / 1000000.f < ms))
        std::this_thread::yield();
    curDF -= gap;
    // showAllInfo();
    df1 = false;
}

void Job::conSubDF(double df, int ms) {
    df2 = true;
    // std::lock_guard<std::mutex> lk(mud2);
    int gap;
    if (curDF - df < 0) {
        gap = curDF;
        curDF = 0;
    } else {
        gap = df;
        curDF -= df;
    }
    // showAllInfo();
    auto start = std::chrono::system_clock::now();
    while (sdf2 && ((std::chrono::system_clock::now() - start).count() / 1000000.f < ms))
        std::this_thread::yield();
    curDF += gap;
    // showAllInfo();
    df2 = false;
}

void Job::addAP(double ap, int ms) {
    std::thread t(&Job::conAddAP, this, ap, ms);
    t.detach();
}

void Job::subAP(double ap, int ms) {
    std::thread t(&Job::conSubAP, this, ap, ms);
    t.detach();
}

void Job::conAddAP(double ap, int ms) {
    ap1 = true;
    // std::lock_guard<std::mutex> lk(mua1);
    int gap;
    if (curAP + ap > MAXATK) {
        gap = MAXATK - curAP;
        curAP = MAXATK;
    } else {
        gap = ap;
        curAP += ap;
    }
    // showAllInfo();
    auto start = std::chrono::system_clock::now();
    while (sap1 && ((std::chrono::system_clock::now() - start).count() / 1000000.f < ms))
        std::this_thread::yield();
    curAP -= gap;
    // showAllInfo();
    ap1 = false;
}

void Job::conSubAP(double ap, int ms) {
    ap2 = true;
    // std::lock_guard<std::mutex> lk(mua2);
    int gap;
    if (curAP - ap < 0) {
        gap = curAP;
        curAP = 0;
    } else {
        gap = ap;
        curAP -= ap;
    }
    // showAllInfo();
    auto start = std::chrono::system_clock::now();
    while (sap2 && ((std::chrono::system_clock::now() - start).count() / 1000000.f < ms))
        std::this_thread::yield();
    curAP += gap;
    // showAllInfo();
    ap2 = false;
}

void Job::showHP() {
    // globalVar::screen->clearMes(infoX, infoY, 20);
    globalVar::screen->setMes(
        "血量: " + std::to_string(int(curHP)) + '/' + std::to_string(int(HP))+std::string(7, ' '), infoX, infoY + 1);
}

void Job::showDF() {
    // globalVar::screen->clearMes(infoX, infoY + 2, 20);
    globalVar::screen->setMes(
        "防禦: " + std::to_string(int(curDF)) + " %" + std::string(7, ' '), infoX, infoY + 2);
}

void Job::showAP() {
    // globalVar::screen->clearMes(infoX, infoY + 3, 20);
    globalVar::screen->setMes(
        "攻擊: " + std::to_string(int(curAP)) + std::string(7, ' '), infoX, infoY + 3);
}

void Job::setInfoPos(int x, int y) {
    infoX = x;
    infoY = y;
}

void Job::showbloodImg() {
    int n = int(static_cast<double>(curHP) / HP * 10);
    n = (n > 10) ? 10 : n;
    n = (n < 1 && curHP > 0) ? 1 : n;
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
    double t = static_cast<double>(curAP) / SHOWATK * 10;
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
    globalVar::screen->setStdCursorPos(0, 0);
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

double Job::addDF(double df) {
    double gap = df;
    if (DF + df > MAXDEF) {
        gap = MAXDEF - DF;
        DF = curDF = MAXDEF;
    } else
        curDF = DF += df;
    return gap;
}

double Job::subDF(double df) {
    double gap = df;
    if (DF - df < 0) {
        gap = DF;
        DF = curDF = 0;
    } else
        curDF = DF -= df;
    return gap;
}

double Job::addAP(double ap) {
    double gap = ap;
    if (AP + ap > MAXATK) {
        gap = MAXATK - AP;
        AP = curAP = MAXATK;
    } else
        curAP = AP += ap;
    return gap;
}

double Job::subAP(double ap) {
    int gap = ap;
    if (AP - ap < 0) {
        gap = AP;
        AP = curAP = 0;
    } else
        curAP = AP += ap;
    return gap;
}

double Job::subHP(double hp) {
    int gap = hp;
    if (HP - hp <= 0) {
        gap = AP-1;
        HP = curHP = 1;
    } else
        curHP = HP += hp;
    return gap;
}

double Job::addHP(double hp) {
    curHP = HP += hp;
    return hp;
}

void Job::chCurDF(double df) {
    curDF += df;
    DF += df;
}

void Job::chCurAP(double ap) {
    curAP += ap;
    AP += ap;
}

void Job::chCurHP(double hp) {
    HP += hp;
    curHP += hp;
    if (curHP <= 0)
        curHP = 1;
}

#endif
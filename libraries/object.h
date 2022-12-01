#ifndef OBJECT_H
#define OBJECT_H 1

#include <atomic>
#include <mutex>
#include <thread>

#include "global.h"
#include "map.h"

#define MAXATK 100
#define MAXDEF 100
#define MAXBLOOD 10000

static std::mutex s_mutex, mud1, mud2, mua1, mua2;

class Object {
   private:
    int HP;  // 血量
    int AP;  // 傷害
    int DF;
    int curHP;
    int curAP;
    int curDF;
    int infoX;
    int infoY;
    std::atomic<bool> sdf1, sdf2, sap1, sap2;
    std::atomic<bool> df1, df2, ap1, ap2;

   public:
    Object(int hp, int df, int ap);

    inline int upDef(int df);
    inline int upHP(int hp);
    inline int upAp(int ap);
    inline int healHP(int heal);
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

Object::Object(int hp, int df, int ap) {
    curHP = HP = hp;
    curDF = DF = df;
    curAP = AP = ap;
    sdf1 = sdf2 = sap1 = sap2 = true;
}

int Object::healHP(int heal) {
    return curHP = (curHP + heal > HP) ? HP : curHP + heal;
}

bool Object::deHP(int atk) {
    atk = round(atk * (1 - (double)curDF / MAXDEF));
    if (curHP - atk <= 0) {
        curHP = 0;
        return false;
    }
    curHP -= atk;
    return true;
}

void Object::addDF(int df, int ms) {
    std::thread t(&Object::conAddDF, this, df, ms);
    t.detach();
}

void Object::subDF(int df, int ms) {
    std::thread t(&Object::conSubDF, this, df, ms);
    t.detach();
}

void Object::conAddDF(int df, int ms) {
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

void Object::conSubDF(int df, int ms) {
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

void Object::addAP(int ap, int ms) {
    std::thread t(&Object::conAddAP, this, ap, ms);
    t.detach();
}

void Object::subAP(int ap, int ms) {
    std::thread t(&Object::conSubAP, this, ap, ms);
    t.detach();
}

void Object::conAddAP(int ap, int ms) {
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

void Object::conSubAP(int ap, int ms) {
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

int Object::upDef(int df) {
    return curDF = DF += df;
}

int Object::upHP(int hp) {
    return curHP = HP += hp;
}

int Object::upAp(int ap) {
    return curAP = AP += ap;
}

void Object::showHP() {
    globalVar::screen->clearMes(infoX, infoY, 12);
    globalVar::screen->setMes("血量: " + std::to_string(curHP) + '/' + std::to_string(HP), infoX, infoY + 1);
}

void Object::showDF() {
    globalVar::screen->clearMes(infoX, infoY + 3, 12);
    globalVar::screen->setMes("防禦: " + std::to_string(curDF) + " %", infoX, infoY + 3);
}

void Object::showAP() {
    globalVar::screen->clearMes(infoX, infoY + 2, 12);
    globalVar::screen->setMes("攻擊: " + std::to_string(curAP), infoX, infoY + 2);
}

void Object::setInfoPos(int x, int y) {
    infoX = x;
    infoY = y;
}

void Object::showbloodImg() {
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

void Object::showDefImg() {
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

void Object::showAtkImg() {
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

void Object::showAllInfo(SHORT x, SHORT y) {
    std::lock_guard<std::mutex> lk(s_mutex);
    globalVar::screen->setColor(2);
    globalVar::screen->setMes("Player info", x, y);
    globalVar::screen->setColor();
    setInfoPos(x, y + 1);
    showHP();
    showAP();
    showDF();
    showbloodImg();
    showDefImg();
    showAtkImg();
}

void Object::clearAllStatus() {
    clearSubAP();
    clearAddAP();
    clearSubDF();
    clearAddDF();
}

void Object::clearSubDF() {
    sdf2 = false;
    Sleep(10);
    while (df2)
        ;
    sdf2 = true;
}

void Object::clearSubAP() {
    sap2 = false;
    Sleep(10);
    while (ap2)
        ;
    sap2 = true;
}

void Object::clearAddDF() {
    sdf1 = false;
    Sleep(10);
    while (df1)
        ;
    sdf1 = true;
}

void Object::clearAddAP() {
    sap1 = false;
    Sleep(10);
    while (ap1)
        ;
    sap1 = true;
}

#endif
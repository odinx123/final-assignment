#ifndef MONSTER_H
#define MONSTER_H

#include "C110152318_job.h"

void deleteMonsterByID(int ID);

class Monster : private Job {
   private:
    int number = 0;
    std::string name;
    double EXP;
    int itemFallNumber = -1;
    double fallCoin = 0;
    int fallItemNum = -1;
    double fallRate;

   public:
    Monster(std::string n, double hp, double df, double ap,
        int nb, double exp, double fc, int fiN, double fr);

    double getAtk_m() const { return getCurAP(); }
    inline void showInfo_m(SHORT x = 60, SHORT y = 4);
    inline void clearInfo_m(SHORT x = 60, SHORT y = 4);
    std::string getMonstName() const { return name; }
    int getNumber_m() const { return number; }
    int getCurHP_m() const { return getCurHP(); }
    int getCurDF_m() const { return getCurDF(); }
    int getCurAP_m() const { return getCurAP(); }
    double deHP_m(double atk) { return deHP(atk); }
    double getEXP() const { return EXP; }
    double getFallCoin() const { return fallCoin; }
    int getFallItemNum() const { return fallItemNum; }
    double getFallRate() const { return fallRate; }
};

Monster* getMonsterByID(int ID);

Monster::Monster(
    std::string n, double hp, double df, double ap, int nb, double exp, double fc, int fiN, double fr)
    : Job(hp, df, ap), name(n), number(nb), EXP(exp), fallCoin(fc), fallItemNum(fiN), fallRate(fr) {}

void Monster::showInfo_m(SHORT x, SHORT y) {
    globalVar::screen->setMes(name+std::string(10, ' '), x, y);
    showAllInfo(x, y);
}

// todo
void Monster::clearInfo_m(SHORT x, SHORT y) {
    for (int i = 0; i < 8+1; ++i) {
        globalVar::screen->clearMes(x, y+i, 25);
    }
}

Monster* getMonsterByID(int ID) {
    Monster* curMonster = nullptr;
    for (auto m : globalVar::monsterList) {
        if (ID == m->getNumber_m()) {
            curMonster = m;
            break;
        }
    }
    return curMonster;
}

void deleteMonsterByID(int ID) {
    for (auto it = globalVar::monsterList.begin(); it != globalVar::monsterList.end(); ++it) {
        if (ID == (*it)->getNumber_m()) {
            delete *it;
            globalVar::monsterList.erase(it);
            break;
        }
    }
}

#endif
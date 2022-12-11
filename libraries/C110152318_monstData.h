#ifndef MONSTERDATA_H
#define MONSTERDATA_H 1

#include <string>

#include "C110152318_global.h"
#include "C110152318_map.h"
#include "C110152318_monster.h"

#define MONSIZE 10

double totle = 0;
int NUM;

struct MonstData {
    double HP;
    double DF;
    double AP;
    double EXP;
    double apear;
    std::string name;
    static int curMonsNum;
    MonstData(std::string n, double hp, double df, double ap, double exp, double a)
        : name(n), HP(hp), DF(df), AP(ap), EXP(exp), apear(a) {}
};

int MonstData::curMonsNum = 0;
int gapSize_ = 0, gapHP = 0, gapDF = 0, gapAP = 0;

MonstData ranInfo[] = {  // todo
    // 名稱、血量、防禦、傷害、EXP、出現率(按比例計算)、金錢(todo)、掉裝(-1不掉裝)  todo
    MonstData("千年妖精", 100000, 80, 400, 3479, 1),
    MonstData("莽原射手", 32349, 10, 783, 4823, 4),
    MonstData("兵馬俑", 8458, 6, 120, 1302, 10),
    MonstData("殭屍", 20, 5, 10, 53, 30),
    MonstData("小妖", 234, 6, 36, 234, 25),
    MonstData("木樁", 10000000, 30, 1, 1, 20),
    MonstData("魔王", 73842983, 30, 1599, 12045, -1),
    MonstData("陵寢巫師", 603845, 10, 1299, 8598, -1)
};

void genAnyNumMons(int n);
void initMonsOccurProb();

void genAnyNumMons(int n) {
    int pos = 0;
    for (int i = 0; i < n; ++i) {
        for (pos = 0; pos < NUM; ++pos)
            if (rand() / (RAND_MAX + 1.0) <= globalVar::chance_m[pos]) break;

        globalVar::monsterList.emplace_back(new Monster(
            ranInfo[pos].name, ranInfo[pos].HP, ranInfo[pos].DF, ranInfo[pos].AP, MonstData::curMonsNum, ranInfo[pos].EXP));

        int size_ = gapSize_ / 3 * 2 - ranInfo[pos].name.size() / 3 * 2;
        int sizeHP = gapHP - getNumberSize(ranInfo[pos].HP) + 2;
        int sizeAP = gapAP - getNumberSize(ranInfo[pos].AP) + 2;
        int sizeDF = gapDF - getNumberSize(ranInfo[pos].DF) + 2;
        globalVar::screen->printMapMes(
            ranInfo[pos].name + "(" + std::to_string(MonstData::curMonsNum++) + ")" +
            std::string(size_, ' ') + "【 HP:" + std::to_string(int(ranInfo[pos].HP)) +
            std::string(sizeHP, ' ') + "DF:" + std::to_string(int(ranInfo[pos].DF)) +
            std::string(sizeDF, ' ') + "AP:" + std::to_string(int(ranInfo[pos].AP)) + std::string(sizeAP, ' ') + "】");
    }
    globalVar::screen->printMapMes(" ");
}

void initMonsOccurProb() {
    NUM = sizeof(ranInfo) / sizeof(MonstData);
    for (int i = 0; i < NUM; ++i) {
        totle += (ranInfo[i].apear >= 0 ? ranInfo[i].apear : 0);
        gapSize_ = gapSize_ > (int)ranInfo[i].name.size() ? gapSize_ : ranInfo[i].name.size();
        gapHP = gapHP >= getNumberSize(ranInfo[i].HP) ? gapHP : getNumberSize(ranInfo[i].HP);
        gapAP = gapAP >= getNumberSize(ranInfo[i].AP) ? gapAP : getNumberSize(ranInfo[i].AP);
        gapDF = gapDF >= getNumberSize(ranInfo[i].DF) ? gapDF : getNumberSize(ranInfo[i].DF);
    }
    double *temp = new double[NUM]();
    for (int i = 0; i < NUM; ++i) {
        temp[i] = ranInfo[i].apear / totle;
        if (!i)
            globalVar::chance_m.push_back(temp[i]);
        else
            globalVar::chance_m.push_back(temp[i] + globalVar::chance_m[i - 1]);
    }
    delete[] temp;
}

#endif
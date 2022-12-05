#ifndef OBJECT_H
#define OBJECT_H

#include <string>

#include "C110152318_user.h"
#include "C110152318_global.h"

class Object {
   private:
    std::string name;
    int addHP;
    int addDF;
    int addAP;
    int reHP;
    int reDF;
    int reAP;
    int ms;
    int coin;

   public:
    Object(const std::string& s, int hp = 0, int df = 0, int ap = 0, int t = 0, int c = 100);

    void wareObj(bool s);
    inline void printName(SHORT x, SHORT y) const;
    std::string getName() const { return name; }
    int getCoin() const { return coin; }
};

Object::Object(const std::string& s, int hp, int df, int ap, int t, int c)
    : addAP(ap), addDF(df), addHP(hp), name(s), ms(t), coin(c) {
    reAP = reHP = reDF = 0;
}

void Object::wareObj(bool s) {
    if (!s) {
        globalVar::user->jb->subAP(reAP);
        globalVar::user->jb->subHP(reHP);
        globalVar::user->jb->subDF(reDF);
        globalVar::screen->printMapMes("你脫下了<" + name + ">");
        globalVar::screen->printMapMes(
            "HP:" + std::to_string(addHP) + " " +
            "AP:" + std::to_string(addAP) + " " +
            "DF:" + std::to_string(addDF));
        reAP = reHP = reDF = 0;
    } else {
        reAP = globalVar::user->jb->addAP(addAP);
        reHP = globalVar::user->jb->addHP(addHP);
        reDF = globalVar::user->jb->addDF(addDF);
        globalVar::screen->printMapMes("你裝備了<" + name + ">");
        globalVar::screen->printMapMes(
            "HP:" + std::to_string(addHP) + " " +
            "AP:" + std::to_string(addAP) + " " +
            "DF:" + std::to_string(addDF));
    }
}

void Object::printName(SHORT x, SHORT y) const {
    globalVar::screen->clearMes(x, y, 20);
    globalVar::screen->setMes(name, x, y);
}

#endif
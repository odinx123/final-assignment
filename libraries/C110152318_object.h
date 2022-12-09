#ifndef OBJECT_H
#define OBJECT_H

#include <string>

#include "C110152318_global.h"
#include "C110152318_user.h"

class Object {
   private:
    std::string name;
    int addHP;
    int addDF;
    int addAP;
    int ms;
    int coin;
    int type;
    int idx;

   public:
    Object(const std::string& s, int hp = 0, int df = 0, int ap = 0, int t = 0, int c = 100, int ty = 0, int id = 0);

    void wareObj(bool s);
    inline void printName(SHORT x, SHORT y) const;
    std::string getName() const { return name; }
    int getCoin() const { return coin; }
    int useObj();
    // return 0 for eat, 1 for use.
    inline int useBagObj(bool s);
    int getIdx() const { return idx; }
    // 0 for eat, 1 for use.
    int getType() const { return type; }
};

Object::Object(const std::string& s, int hp, int df, int ap, int t, int c, int ty, int id)
    : addAP(ap), addDF(df), addHP(hp), name(s), ms(t), coin(c), type(ty), idx(id) {
}

void Object::wareObj(bool s) {
    if (!s && globalVar::user->wareNumber == idx) {
        globalVar::user->jb->chCurAP(-addAP);
        globalVar::user->jb->chCurHP(-addHP);
        globalVar::user->jb->chCurDF(-addDF);
        globalVar::screen->printMapMes("你脫下了<" + name + ">");
        globalVar::screen->printMapMes(
            "HP:" + std::to_string(addHP) + " " +
            "AP:" + std::to_string(addAP) + " " +
            "DF:" + std::to_string(addDF));
    } else if (!globalVar::user->ware) {
        globalVar::user->jb->chCurAP(addAP);
        globalVar::user->jb->chCurHP(addHP);
        globalVar::user->jb->chCurDF(addDF);
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

int Object::useObj() {
    if (addHP < 0)
        globalVar::user->jb->subHP(addHP);
    else
        globalVar::user->jb->healHP(addHP);
    if (addDF < 0)
        globalVar::user->jb->subDF(addDF, ms);
    else
        globalVar::user->jb->addDF(addDF, ms);
    if (addAP < 0)
        globalVar::user->jb->subAP(addAP, ms);
    else
        globalVar::user->jb->addAP(addAP, ms);
    globalVar::screen->printMapMes("你吃下了了<" + name + ">");
    globalVar::screen->printMapMes(
        "HP:" + std::to_string(addHP) + "\t" +
        "<AP:" + std::to_string(addAP) + " " +
        "DF:" + std::to_string(addDF) + "  改變" + std::to_string(ms/1000)+"秒>");
    globalVar::user->showInfo();
    return type;
}

// bug
int Object::useBagObj(bool s) {
    if (type) wareObj(s);
    else if (s) useObj();
    return type;
}

#endif
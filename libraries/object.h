#ifndef OBJECT_H
#define OBJECT_H 1

#include "global.h"
#include "map.h"

class Object {
   private:
    SHORT HP;  // 血量
    SHORT AP;  // 傷害
    SHORT DF;
    SHORT curHP;

   protected:
    SHORT infoX;
    SHORT infoY;

   public:
    Object(SHORT hp, SHORT df, SHORT ap);

    inline SHORT healHp(int heal);
    inline SHORT upDef(int df);
    inline SHORT upHP(int hp);
    inline SHORT upAp(int ap);
    inline void showHP(SHORT x, SHORT y);
    inline void showDF(SHORT x, SHORT y);
    inline void showAP(SHORT x, SHORT y);
};

Object::Object(SHORT hp, SHORT df, SHORT ap) {
    HP = hp;
    DF = df;
    AP = ap;
    curHP = HP;
    infoX = 45;
    infoY = 1;
}

SHORT Object::healHp(int heal) {
    return curHP = (curHP + heal > HP) ? HP : curHP + heal;
}

SHORT Object::upDef(int df) {
    return DF += df;
}

SHORT Object::upHP(int hp) {
    return HP += hp;
}

SHORT Object::upAp(int ap) {
    return AP += ap;
}

void Object::showHP(SHORT x, SHORT y) {
    globalVar::screen->setMes("血量: " + std::to_string(HP) + '/' + std::to_string(curHP), x, y);
}

void Object::showDF(SHORT x, SHORT y) {
    globalVar::screen->setMes("防禦: " + std::to_string(DF), x, y);
}

void Object::showAP(SHORT x, SHORT y) {
    globalVar::screen->setMes("攻擊: " + std::to_string(AP), x, y);
}

#endif
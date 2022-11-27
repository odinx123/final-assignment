#ifndef OBJECT_H
#define OBJECT_H 1

#include "map.h"
#include "global.h"

class Object {
   private:
    SHORT HP;  // 血量
    SHORT AP;  // 傷害
    SHORT DF;
    SHORT curHP;

   public:
    Object(SHORT hp, SHORT df, SHORT ap);
};

Object::Object(SHORT hp, SHORT df, SHORT ap) {
    HP = hp;
    DF = df;
    AP = ap;
    curHP = HP;
}

#endif
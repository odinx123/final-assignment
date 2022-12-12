#ifndef OBJECT_H
#define OBJECT_H

#include <string>

#include "C110152318_global.h"
#include "C110152318_user.h"

#define EAT 0
#define ARMS 1
#define BODY 2
#define LEG 3

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
    std::string jobNumber;

   public:
    static int objNum;
    Object(const std::string& s, int hp = 0, int df = 0, int ap = 0, int t = 0,
           int c = 100, int ty = 0, int id = 0, std::string jbn = 0);
    ~Object();

    void wareObj(bool s);
    inline void printName(SHORT x, SHORT y) const;
    std::string getName() const { return name; }
    int getCoin() const { return coin; }
    int getIdx() const { return idx; }
    // 0 for eat, 1 for use.
    int getType() const { return type; }
    std::string getJobNumber() const { return jobNumber; }
    void use();
    void deuse();
    int eat();
};

int Object::objNum = 0;

Object::Object(const std::string& s, int hp, int df, int ap, int t,
               int c, int ty, int id, std::string jbn)
    : addAP(ap), addDF(df), addHP(hp), name(s), ms(t), coin(c), type(ty), idx(id), jobNumber(jbn) {}

// 物品名、加血量、加防禦、加攻擊、效果時間(裝備無ms)、價錢、類型(0是吃的，1是穿的)、編號、適用職業(通用0)
std::vector<Object*> objList = {
    new Object("金蘋果", 10, 10, 10, 10000, 50, EAT, Object::objNum++, "0"),              // 0
    new Object("木劍", 0, 0, 10, 0, 100, ARMS, Object::objNum++, "0"),                    // 1
    new Object("鐵劍", 0, 0, 60, 0, 1000, ARMS, Object::objNum++, "0"),                   // 2
    new Object("火腿", 10, 10, 10, 20000, 10, EAT, Object::objNum++, "0"),                // 3
    new Object("治療藥水【小】", 100, 0, 0, 0, 500, EAT, Object::objNum++, "0"),            // 4
    new Object("治療藥水【中】", 2000, 0, 0, 0, 5000, EAT, Object::objNum++, "0"),          // 5
    new Object("治療藥水【大】", 5000, 0, 0, 0, 10000, EAT, Object::objNum++, "0"),         // 6
    new Object("鐵裝", 500, 15, 0, 0, 2000, BODY, Object::objNum++, "0"),                 // 7
    new Object("皮革裝", 100, 4, 0, 0, 500, BODY, Object::objNum++, "0"),                 // 8
    new Object("大砍刀【戰士限定】", 100, 10, 100, 0, 1260, ARMS, Object::objNum++, "1"),   // 9
    new Object("瑪娜魔仗【法師限定】", 50, 2, 378, 0, 1260, ARMS, Object::objNum++, "2"),   // 10
    new Object("月辰短劍【刺客限定】", 0, 0, 600, 0, 1260, ARMS, Object::objNum++, "3"),    // 11
    new Object("守護者鎧甲【坦克限定】", 0, 0, 600, 0, 1260, BODY, Object::objNum++, "4"),  // 12
    new Object("長生劍【傳說武器】", 500, 20, 2000, 0, 0, ARMS, Object::objNum++, "0")      // 13
};

Object::~Object() {
    for (auto p : objList)
        delete p;
}

void Object::printName(SHORT x, SHORT y) const {
    globalVar::screen->clearMes(x, y, 20);
    globalVar::screen->setMes(name, x, y);
}

int Object::eat() {
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
        "DF:" + std::to_string(addDF) + "  改變" + std::to_string(ms / 1000) + "秒>");
    globalVar::user->showInfo();
    return type;
}

void Object::use() {  // just for use item not eat.
    globalVar::user->jb->chCurAP(addAP);
    globalVar::user->jb->chCurHP(addHP);
    globalVar::user->jb->chCurDF(addDF);
    globalVar::screen->printMapMes("你裝備了<" + name + ">");
    globalVar::screen->printMapMes(
        "HP:" + std::to_string(addHP) + " " +
        "AP:" + std::to_string(addAP) + " " +
        "DF:" + std::to_string(addDF));
}

void Object::deuse() {  // just for use item not eat.
    globalVar::user->jb->chCurAP(-addAP);
    globalVar::user->jb->chCurHP(-addHP);
    globalVar::user->jb->chCurDF(-addDF);
    globalVar::screen->printMapMes("你脫下了<" + name + ">");
    globalVar::screen->printMapMes(
        "HP:" + std::to_string(-addHP) + " " +
        "AP:" + std::to_string(-addAP) + " " +
        "DF:" + std::to_string(-addDF));
}

#endif
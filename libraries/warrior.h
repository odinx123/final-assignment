#ifndef WARRIOR_H
#define WARRIOR_H 1

#include "job.h"

class Warrior : public Job {
   private:
    // init warrior status
    static const SHORT HP = 30;
    static const SHORT DF = 20;
    static const SHORT AP = 10;

   public:
    Warrior(SHORT hp, SHORT df, SHORT ap);

    inline void showInfo(SHORT x = 27, SHORT y = 1);
};

Warrior::Warrior(SHORT hp = HP, SHORT df = DF, SHORT ap = AP) : Job(hp, df, ap) {}

void Warrior::showInfo(SHORT x, SHORT y) {
    globalVar::screen->setColor(10);
    globalVar::screen->setMes("戰士", x, y);
    globalVar::screen->setColor();
    showAllInfo(x, y);
}

#endif
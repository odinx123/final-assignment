#ifndef Job_H
#define Job_H 1

#include "object.h"

class Job : public Object {
   private:

   public:
    Job() = default;
    Job(SHORT hp, SHORT df, SHORT ap);

    inline void showInfo();
};

Job::Job(SHORT hp, SHORT df, SHORT ap) : Object(hp, df, ap) {
}

void Job::showInfo() {
    globalVar::screen->setColor(2);
    globalVar::screen->setMes("Player info", infoX, infoY);
    globalVar::screen->setColor();
    showHP(infoX, infoY + 1);
    showAP(infoX, infoY + 2);
    showDF(infoX, infoY + 3);
}

#endif
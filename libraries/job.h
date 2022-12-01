#ifndef Job_H
#define Job_H 1

#include "object.h"

class Job : public Object {
   private:

   public:
    Job() = default;
    Job(SHORT hp, SHORT df, SHORT ap);

    inline void showInfo(SHORT x = 27, SHORT y = 1);
};

Job::Job(SHORT hp, SHORT df, SHORT ap) : Object(hp, df, ap) {
}

void Job::showInfo(SHORT x, SHORT y) {
    showAllInfo(x, y);
}

#endif
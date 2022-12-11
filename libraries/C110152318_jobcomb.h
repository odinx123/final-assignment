#ifndef WARRIOR_H
#define WARRIOR_H 1

#include "C110152318_job.h"
//戰士, 法師, 刺客, 坦克
const double atk[4] = {0.096, 0.11, 0.13, 0.087};
const double hpu[4] = {0.30, 0.28, 0.26, 0.32};

const double def[4] = {0.0055, 0.006, 0.005, 0.008};

class JobComb : public Job {
   private:
    // init warrior status
    double HP;
    double DF;
    double AP;
    SHORT number;

   public:
    JobComb(double hp, double df, double ap, SHORT n);
    void setLevelStatus();
};

JobComb::JobComb(double hp, double df, double ap, SHORT n) : Job(hp, df, ap) {
    number = n;
}

void JobComb::setLevelStatus() {
    addHP(int(getBaseHP()*0.1+100)*hpu[number]);
    addAP((getBaseAP()*0.1+100)*atk[number]);
    
    addDF((getBaseDF()*0.1+50)*def[number]);
}

#endif
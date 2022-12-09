#ifndef FIGHT_H
#define FIGHT_H 1

#include "C110152318_monster.h"
#include "C110152318_user.h"
#include "C110152318_monstData.h"

class Fight {
   public:
    Fight();

    void getFightWithMonsId(int id);
};

Fight::Fight() {
}

void Fight::getFightWithMonsId(int id) {
    bool state = true;
    bool atkPri = (rand() % 5) != 0;

    Monster* curMonster = getMonsterByID(id);
    if (curMonster == nullptr) {
        globalVar::screen->printMapMes("沒有這個怪物!!!");
        return;
    }
    curMonster->showInfo_m();
    Sleep(1500);
    while (state) {
        double playerAtk = globalVar::user->jb->getCurAP();
        double monsterAtk = curMonster->getCurAP_m();
        if (_kbhit() && _getch() == 27) {
            globalVar::screen->printMapMes("你逃跑了!!!");
            break;
        }
        if (atkPri) {
            if (rand()%7) {  // 普攻
                double dmg = curMonster->deHP_m(playerAtk);
                globalVar::screen->printMapMes(
                    "你出手攻擊對【 "+curMonster->getMonstName()+
                    " 】造成< "+std::to_string(dmg)+" >點傷害!!!");
            } else {  // 爆擊
                double dmg = curMonster->deHP_m(playerAtk+playerAtk*0.5);
                globalVar::screen->printMapMes(
                    "你憤力一擊對【 "+curMonster->getMonstName()+
                    " 】造成< "+std::to_string(dmg)+" >點爆擊傷害!!!");
            }
            
            curMonster->showInfo_m();
            if (curMonster->getCurHP_m() <= 0.5) {  // 死掉
                --MonstData::curMonsNum;
                globalVar::screen->printMapMes(curMonster->getMonstName()+"死亡!");
                globalVar::user->expUp(curMonster->getEXP());
                deleteMonsterByID(id);
                break;
            }
        } else {
            if (rand()%7) {  // 普攻
                double dmg = globalVar::user->jb->deHP(monsterAtk);
                globalVar::screen->printMapMes(
                    "【 "+curMonster->getMonstName()+" 】出手攻擊對你造成< "+
                    std::to_string(dmg)+" >點傷害!!!");
            } else {  // 爆擊
                double dmg = globalVar::user->jb->deHP(monsterAtk+monsterAtk*0.5);
                globalVar::screen->printMapMes(
                    "【 "+curMonster->getMonstName()+" 】出手攻擊產生【爆擊】對你造成< "+
                    std::to_string(dmg)+" >點傷害!!!");
            }
            
            globalVar::user->showInfo();
            if (globalVar::user->jb->getCurHP() <= 0.5) {  // 死掉
                globalVar::user->jb->setCurHP(0);
                globalVar::screen->printMapMes("你已經死亡!");
                globalVar::user->setLive(false);
                globalVar::gameState = false;
                Sleep(2000);
                break;
            }
        }
        globalVar::screen->printMapMes("==========================================================");
        Sleep(500);
        atkPri = !atkPri;
    }
    Sleep(700);
    globalVar::screen->printMapMes(" ");
    curMonster->clearInfo_m();
}

#endif
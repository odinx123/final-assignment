#ifndef FIGHT_H
#define FIGHT_H 1

#include "C110152318_monstData.h"
#include "C110152318_monster.h"
#include "C110152318_user.h"
#include "conio.h"

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
    int atkNum = 0, suckBlood = 0;

    Monster* curMonster = getMonsterByID(id);
    if (curMonster == nullptr) {
        globalVar::screen->printMapMes("沒有這個怪物!!!");
        return;
    }
    curMonster->showInfo_m();
    Sleep(1500);
    while (state) {
        globalVar::screen->setCursorVisible(false);
        globalVar::screen->clearAllMap();
        globalVar::screen->displayMap();
        globalVar::user->showInfo();
        double playerAtk = globalVar::user->jb->getCurAP();
        double monsterAtk = curMonster->getCurAP_m();
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) {  // ESC
                globalVar::screen->printMapMes("你逃跑了!!!");
                globalVar::screen->printMapMes(" ");
                break;
            } else if (ch == 32) {  // Space
                int sk = globalVar::user->useSkill();
                if (sk == -1) {
                    globalVar::screen->printMapMes("技能點數不夠!!!");
                    globalVar::screen->printMapMes(" ");
                    Sleep(1000);
                    continue;
                }
                if (sk == 0) atkNum += 4*globalVar::user->getSklevel();
                else if (sk == 1);
                else if (sk == 2) suckBlood += 5;
                else if (sk == 3);
            }
        }
        if (atkNum) {
            --atkNum;
            atkPri = true;
        }
        if (atkPri) {
            double dmg = 0;
            if (rand() % 4) {  // 普攻
                dmg = curMonster->deHP_m(playerAtk);
                globalVar::screen->printMapMes(
                    "你出手攻擊對【 " + curMonster->getMonstName() +
                    " 】造成< " + std::to_string(dmg) + " >點傷害!!! 【普攻】");
            } else if (!(rand() % 5)) {  // 閃避
                globalVar::screen->printMapMes(
                    "你發動攻擊但【" + curMonster->getMonstName() + "】身手敏捷躲避了攻擊!!! 【閃躲】");
            } else {  // 爆擊
                dmg = curMonster->deHP_m(playerAtk + playerAtk * 0.5);
                globalVar::screen->printMapMes(
                    "你憤力一擊對【 " + curMonster->getMonstName() +
                    " 】造成< " + std::to_string(dmg) + " >點爆擊傷害!!! 【爆擊】");
            }
            if (suckBlood) {
                --suckBlood;
                double heblood = dmg*(10*globalVar::user->getSklevel()/100.0);
                globalVar::user->jb->healHP(heblood);
                if (dmg == 0) {
                    globalVar::screen->printMapMes("你的攻擊揮空,寫量沒有恢復!!!");
                } else {
                    globalVar::screen->printMapMes(
                    "你擁有【吸血】效果,寫量恢復"+
                    std::to_string(int(heblood)));
                }
            }

            curMonster->showInfo_m();
            if (curMonster->getCurHP_m() <= 0.5) {  // 殺掉
                ++globalVar::killNum;
                ++globalVar::user->totleKill;
                ++globalVar::user->skillPoint;
                globalVar::screen->printMapMes(curMonster->getMonstName() + "死亡!");
                globalVar::user->expUp(curMonster->getEXP());
                deleteMonsterByID(id);
                break;
            }
        } else {
            if (rand() % 4) {  // 普攻
                double dmg = globalVar::user->jb->deHP(monsterAtk);
                globalVar::screen->printMapMes(
                    "【 " + curMonster->getMonstName() + " 】出手攻擊對你造成< " +
                    std::to_string(dmg) + " >點傷害!!! 【普攻】");
            } else if (!(rand() % 5)) {  // 閃避
                globalVar::screen->printMapMes(
                    "【"+curMonster->getMonstName()+"】發動攻擊,但你身手敏捷躲避了攻擊!!! 【閃躲】");
            } else {  // 爆擊
                double dmg = globalVar::user->jb->deHP(monsterAtk + monsterAtk * 0.5);
                globalVar::screen->printMapMes(
                    "【 " + curMonster->getMonstName() + " 】出手攻擊產生對你造成< " +
                    std::to_string(dmg) + " >點傷害!!! 【爆擊】");
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
        globalVar::screen->printMapMes(" ");
        Sleep(1000);
        atkPri = !atkPri;
    }
    Sleep(700);
    globalVar::screen->printMapMes(" ");
    curMonster->clearInfo_m();
}

#endif
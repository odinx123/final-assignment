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
    int run = 0;

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
                if (sk == 0)
                    atkNum += 4 * globalVar::user->getSklevel();
                else if (sk == 1)
                    ;
                else if (sk == 2)
                    suckBlood += 5 * globalVar::user->getSklevel();
                else if (sk == 3)
                    ;
            }
        }
        if (run == 4) {  // 五回合
            run = 0;
            globalVar::user->incSkillPoint();
        } else if (!globalVar::user->isSkillFull()) {
            ++run;
        }

        if (atkNum) {
            --atkNum;
            atkPri = true;
        }
        if (atkPri) {
            double dmg = 0;
            double rate = rand() / (RAND_MAX + 1.0);
            if (rate <= globalVar::user->getCritiCalRate()) {  // 爆擊
                dmg = curMonster->deHP_m(playerAtk + playerAtk * globalVar::user->getCritiCalDmg());
                globalVar::screen->printMapMes(
                    "你憤力一擊對【 " + curMonster->getMonstName() +
                    " 】造成< " + std::to_string(dmg) + " >點爆擊傷害!!! 【爆擊】");
            } else if (rate < 0.95) {  // 普攻
                dmg = curMonster->deHP_m(playerAtk);
                globalVar::screen->printMapMes(
                    "你出手攻擊對【 " + curMonster->getMonstName() +
                    " 】造成< " + std::to_string(dmg) + " >點傷害!!! 【普攻】");
            } else {  // 閃避
                globalVar::screen->printMapMes(
                    "你發動攻擊但【" + curMonster->getMonstName() + "】身手敏捷躲避了攻擊!!! 【閃躲】");
            }
            // if (!(run--))
            //     globalVar::user->incSkillPoint();

            if (suckBlood) {
                --suckBlood;
                double heblood = dmg * (10 * globalVar::user->getSklevel() / 100.0);
                globalVar::user->jb->healHP(heblood);
                if (dmg == 0) {
                    globalVar::screen->printMapMes("你的攻擊揮空,血量沒有恢復!!!");
                } else {
                    globalVar::screen->printMapMes(
                        "你擁有【吸血】效果,血量恢復" +
                        std::to_string(int(heblood)));
                }
            }

            curMonster->showInfo_m();
            if (curMonster->getCurHP_m() <= 0.5) {  // 殺掉
                if (rand()/(RAND_MAX+1.0) <= curMonster->getFallRate()) {  // 掉裝
                    globalVar::screen->printMapMes(
                        "成功掉落【"+
                        objList[curMonster->getFallItemNum()]->getName()+"】");
                    globalVar::bg->addBagItemByIdx(curMonster->getFallItemNum(), 1);
                }
                globalVar::user->changeCoin(curMonster->getFallCoin());  // 獲得錢
                ++globalVar::killNum;  // 用來判斷是否生怪用的
                ++globalVar::user->totleKill;  // 增加總擊殺數
                globalVar::user->expUp(curMonster->getEXP());  // 獲得經驗
                globalVar::screen->printMapMes(curMonster->getMonstName() + "死亡!");
                deleteMonsterByID(id);  // 刪除怪物
                break;
            }
        } else {
            if (rand()/(RAND_MAX+1.0) < 0.8) {  // 普攻
                double dmg = globalVar::user->jb->deHP(monsterAtk);
                globalVar::screen->printMapMes(
                    "【 " + curMonster->getMonstName() + " 】出手攻擊對你造成< " +
                    std::to_string(dmg) + " >點傷害!!! 【普攻】");
            } else if (!(rand() % 6)) {  // 閃避
                globalVar::screen->printMapMes(
                    "【" + curMonster->getMonstName() + "】發動攻擊,但你身手敏捷躲避了攻擊!!! 【閃躲】");
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
        globalVar::screen->printMapMes("==============================================================");
        globalVar::screen->printMapMes(" ");
        Sleep(1000);
        atkPri = !atkPri;
    }
    Sleep(700);
    globalVar::screen->printMapMes(" ");
    curMonster->clearInfo_m();
}

#endif
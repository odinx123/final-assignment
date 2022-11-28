#ifndef LIFEENTITY_H
#define LIFEENTITY_H

#include <string>

#define MAXBLOODBARLEN 40
using namespace std;

class CLifeEntity {
   public:
    CLifeEntity(int initHP = 0, int initSP = 0, string initname = "");
    void setInitSPHP(int initHP = 0, int initSP = 0);
    bool isdead();
    void setHP(int);
    void addHP(int);
    int getHP();
    int getMAXHP();
    int getSP();
    int getMAXSP();
    void addSP(int);
    void delSP(int);
    int gethurt(int hurt);
    string getname();
    void setname(string);
    // CWeapon *getWeapon() { return weapon; };
    // void setWeapon(CWeapon *in_weapon) { weapon = in_weapon; }
    bool kill(CLifeEntity *);
    virtual int attack(CLifeEntity *) = 0;
    virtual int defense(CLifeEntity *) = 0;
    virtual int isA() = 0;

   private:
    void fightstatus(CLifeEntity *f, CLifeEntity *s);
    void bloodbarshow(string title, int maxvalue, int value);
    int maxSP, maxHP;
    int SP, HP;
    string Name;
    // CWeapon *weapon;
};

CLifeEntity::CLifeEntity(int initHP, int initSP, string initname) {
    maxSP = SP = initSP;
    maxHP = HP = initHP;
    Name = initname;
    // weapon = NULL;
}

void CLifeEntity::setInitSPHP(int initHP, int initSP) {
    maxSP = SP = initSP;
    maxHP = HP = initHP;
}

void CLifeEntity::setHP(int inHP) {
    HP = inHP > maxHP ? maxHP : inHP;
}

void CLifeEntity::addHP(int inHP) {
    HP = (inHP + HP) > maxHP ? maxHP : (HP + inHP);
}

void CLifeEntity::addSP(int inSP) {
    SP = inSP + SP;
}

void CLifeEntity::delSP(int inSP) {
    SP -= inSP;
}

int CLifeEntity::getHP() {
    return (HP > 0 ? HP : 0);
}

int CLifeEntity::getMAXHP() {
    return (maxHP);
}

int CLifeEntity::getSP() {
    return SP;
}

int CLifeEntity::getMAXSP() {
    return (maxSP);
}

bool CLifeEntity::isdead() {
    return (HP <= 0);
}

int CLifeEntity::gethurt(int hurt) {
    if (hurt > HP)
        setHP(0);
    else if (hurt > 0)
        setHP(HP - hurt);
    return HP;
}

string CLifeEntity::getname() {
    return Name;
}

void CLifeEntity::setname(string inname) {
    Name = inname;
}

bool CLifeEntity::kill(CLifeEntity *enemy) {
    int f_damage = 0, s_damage = 0;
    CLifeEntity *first, *second;
    int whofirst;
    while (!this->isdead() && !enemy->isdead()) {
        whofirst = rand() % 2;
        if (whofirst == 0) {
            cout << "���m�o�����A���X��ˤH" << endl;
            first = (CLifeEntity *)enemy;
            second = (CLifeEntity *)this;
        } else {
            cout << "�A�m�o�����A���X��ˤH" << endl;
            first = (CLifeEntity *)this;
            second = (CLifeEntity *)enemy;
        }

        s_damage = first->attack(second);
        fightstatus(enemy, this);
        if (second->isdead()) {
            Sleep(1000);
            break;
        }
        f_damage = second->attack(first);
        fightstatus(enemy, this);
        Sleep(2000);
    }
    if (this->isdead())
        return 0;
    return 1;
}

void CLifeEntity::fightstatus(CLifeEntity *f, CLifeEntity *s) {
    cout << endl
         << f->getname() << endl;
    bloodbarshow(string("HP"), f->getMAXHP(), f->getHP());
    cout << s->getname() << endl;
    bloodbarshow(string("HP"), s->getMAXHP(), s->getHP());
    cout << endl;
}

void CLifeEntity::bloodbarshow(string title, int maxvalue, int value) {
    cout << title << "     |";
    float hpslotlen = (float)maxvalue / MAXBLOODBARLEN;
    int numhp = (int)ceil(value / hpslotlen);
    for (int i = 0; i < numhp; i++) {
        cout << "#";
    }
    numhp = (int)floor((maxvalue - value) / hpslotlen);
    for (int i = 0; i < numhp; i++) {
        cout << " ";
    }
    cout << "|" << endl;
}

#endif
#ifndef BAG_H
#define BAG_H 1

#include <nlohmann/json.hpp>
#include <unordered_set>
#include <vector>

#include "C110152318_object.h"

#define MAXBAGNUM 5
#define EAT 0
#define WARE 1

class Bag {
   private:
    std::string path;
    std::vector<Object*> shopList;
    // {編號, {數量, Lv}}
    std::map<int, std::pair<int, int>> storeHouse;
    // {編號, {數量, Lv}}
    std::map<int, std::pair<int, int>> bagList;
    int weapon = -1;                      // 武器
    int chestplate = -1;                  // 胸甲
    int pants = -1;                       // 褲子
    std::unordered_set<int> alreadyWare;  // save it id

   public:
    Bag();

    inline void showShopList() const;
    inline void showBagList() const;
    // return 0 for ok, 1 for money not enought, 2 for command error, 3 for over size.
    int buy(int n);
    std::string getShopNameByIndex(int idx) { return shopList[idx]->getName(); }
    // return -1 失敗
    int useBagItem(int idx, bool s);
    int getBagNum() { return bagList.size(); }
    bool isBagCanUse() { return bagList.size() != 0; }
    void saveBagItem();
    inline void eraseBagItemFromData(int idx);
    void loadBagItem();
    // 0 for eat, 1 for use.
    int getTypeByIdx(int idx) { return shopList[idx]->getType(); }
    inline void throwOut(int idx);
    inline void addBagItemByIdx(int idx);
    // 成功穿上裝備回傳0，反之則為-1
    int wareItem(int index);
    // 成功脫下裝備回傳0，反之則為-1，-2則是裝備不對應
    int deWareItem(int index);
    inline void eatObj(int index);
    void putItemToStoreHouse(int idx);
    void remItemFromStoreHouse(int idx);
    void getObjFromStoreHouse(int idx);
    void showStoreHouse() const;
    int getStoreNum() { return storeHouse.size(); }
};

Bag::Bag() {
    get_path(path);
    path += "\\player\\user.json";
    globalVar::file_in.open(path);
    globalVar::file_in >> globalVar::jin;
    globalVar::file_in.close();
    // 物品名、加血量、加防禦、加攻擊、效果時間(裝備無ms)、價錢、類型(0是吃的，1是穿的)、編號
    // shopList.push_back(objList[0]);
    shopList.push_back(objList[1]);
    shopList.push_back(objList[2]);
    shopList.push_back(objList[3]);
    shopList.push_back(objList[4]);
    shopList.push_back(objList[5]);
    shopList.push_back(objList[6]);
    shopList.push_back(objList[7]);
    shopList.push_back(objList[8]);
    shopList.push_back(objList[9]);
    shopList.push_back(objList[10]);
    shopList.push_back(objList[11]);
    shopList.push_back(objList[12]);
    loadBagItem();
}

void Bag::showShopList() const {
    int cnt = 0;
    for (const auto& s : shopList)
        globalVar::screen->printMapMes(
            std::to_string(cnt++) + ": " + s->getName() + " $" + std::to_string(s->getCoin()));
}

void Bag::showBagList() const {
    // int cnt = 0;
    if (bagList.size() == 0) {
        globalVar::screen->printMapMes("Bag is empty!!!");
        return;
    }
    globalVar::screen->printMapMes("背包格子數量: " + std::to_string(bagList.size()) + "/" + std::to_string(MAXBAGNUM));
    std::string name, space;
    for (const auto& s : bagList) {
        name = objList[s.first]->getName();
        int n = 22 - name.size() / 3 * 2;
        globalVar::screen->printMapMes(
            std::to_string(s.first) + ": " + name + std::string(n, ' ') +
            "數量: " + std::to_string(s.second.first) + "  Lv:" +std::to_string(s.second.second) +
            (alreadyWare.count(objList[s.first]->getIdx()) >= 1 ? " 已裝備" : ""));
    }
}

int Bag::buy(int n) {
    if (n < 0 || n >= shopList.size()) return 2;
    if (bagList.size() >= MAXBAGNUM) return 3;
    if (globalVar::user->getCoin() < shopList[n]->getCoin()) return 1;
    if (shopList[n]->getJobNumber() != globalVar::user->getCurJob() &&
        shopList[n]->getJobNumber() != "0") return 4;
    globalVar::user->changeCoin(-shopList[n]->getCoin());
    globalVar::user->showCoin();
    if (bagList.count(shopList[n]->getIdx()) > 0)
        ++bagList[shopList[n]->getIdx()].first;
    else
        bagList.insert({shopList[n]->getIdx(), {1, 1}});
    return 0;
}

int Bag::useBagItem(int idx, bool s) {  // todo
    if (bagList.count(idx) <= 0) {
        globalVar::screen->printMapMes("此裝備不存在!!!");
        globalVar::screen->printMapMes(" ");
        return -1;
    }
    if (objList[idx]->getJobNumber() != globalVar::user->getCurJob() &&
        objList[idx]->getJobNumber() != "0") {
        globalVar::screen->printMapMes("你的職業無法使用此裝備!!!");
        globalVar::screen->printMapMes(" ");
        return -1;
    }

    int state = 0;
    if (objList[idx]->getType() == 0) {
        if (!s) {
            globalVar::screen->printMapMes("這個物品【不會】被穿在身上!!!");
            return -1;
        }
        eatObj(idx);
    } else {
        if (s) {  // 傳進來true就是穿裝命令
            state = wareItem(idx);
            if (state == -1)
                globalVar::screen->printMapMes("這位部位已經有裝備了!!!");
            globalVar::screen->printMapMes(" ");
        } else {
            state = deWareItem(idx);
            if (state == -1)
                globalVar::screen->printMapMes("這個部位沒有穿上裝備!!!");
            else if (state == -2)
                globalVar::screen->printMapMes("你沒有穿上【"+objList[idx]->getName()+"】!!!");
            globalVar::screen->printMapMes(" ");
        }
    }

    return state;
}

void Bag::saveBagItem() {
    globalVar::user->saveData();
    auto& userBag = globalVar::jin["account"][globalVar::user->getCurId()]["job"][globalVar::user->getCurJob()]["bag"];
    auto& store = globalVar::jin["account"][globalVar::user->getCurId()]["store"];

    if (weapon != -1)
        deWareItem(weapon);
    if (chestplate != -1)
        deWareItem(chestplate);
    if (pants != -1)
        deWareItem(pants);

    for (const auto& o : bagList) {
        userBag[std::to_string(o.first)]["num"] = o.second.first;
        userBag[std::to_string(o.first)]["Lv"] = o.second.second;
    }
    for (const auto& o : storeHouse) {
        store[std::to_string(o.first)]["num"] = o.second.first;
        store[std::to_string(o.first)]["Lv"] = o.second.second;
    }

    globalVar::file_out.open(path);
    globalVar::file_out << globalVar::jin.dump(4) << std::endl;
    globalVar::file_out.close();
}

void Bag::eraseBagItemFromData(int idx) {
    if (globalVar::jin["account"][globalVar::user->getCurId()]["bag"].count(std::to_string(idx)) > 0)
        globalVar::jin["account"][globalVar::user->getCurId()]["bag"].erase(std::to_string(idx));
}

// todo  chagne job ware equip.
void Bag::loadBagItem() {
    auto& userID = globalVar::jin["account"][globalVar::user->getCurId()]["job"][globalVar::user->getCurJob()];
    // std::cout << userID.dump(4) << std::endl;
    for (const auto& o : userID["bag"].items())
            bagList.insert({std::stoi(o.key()), {o.value()["num"], o.value()["Lv"]}});
    for (const auto& o : globalVar::jin["account"][globalVar::user->getCurId()]["store"].items())
        storeHouse.insert({std::stoi(o.key()), {o.value()["num"], o.value()["Lv"]}});
}

void Bag::throwOut(int idx) {
    if (weapon == idx || pants == idx || chestplate == idx) {
        globalVar::screen->printMapMes("你還穿著這件裝備");
        globalVar::screen->printMapMes(" ");
        return;
    }
    globalVar::screen->printMapMes("已丟棄【" + objList[idx]->getName() + "】!!!");
    bagList.erase(idx);
    eraseBagItemFromData(idx);
}

void Bag::addBagItemByIdx(int idx) {
    if (bagList.count(idx) <= 0)
        bagList.insert({idx, {1, 1}});
    else
        ++bagList[idx].first;
}

int Bag::wareItem(int index) {
    switch (objList[index]->getType()) {
        case ARMS:
            if (weapon != -1)
                return -1;
            weapon = index;
            break;
        case BODY:
            if (chestplate != -1)
                return -1;
            chestplate = index;
            break;
        case LEG:
            if (pants != -1)
                return -1;
            pants = index;
            break;
    }
    --bagList[index].first;
    objList[index]->use();
    alreadyWare.insert(index);
    return 0;
}

int Bag::deWareItem(int index) {
    switch (objList[index]->getType()) {
        case ARMS:
            if (weapon == -1)  // todo
                return -1;
            else if (objList[index]->getIdx() != weapon)
                return -2;
            weapon = -1;
            break;
        case BODY:
            if (chestplate == -1)
                return -1;
            else if (objList[index]->getIdx() != chestplate)
                return -2;
            chestplate = -1;
            break;
        case LEG:
            if (pants == -1)
                return -1;
            else if (objList[index]->getIdx() != pants)
                return -2;
            pants = -1;
            break;
    }
    ++bagList[index].first;
    objList[index]->deuse();
    alreadyWare.erase(index);
    return 0;
}

void Bag::eatObj(int index) {
    objList[index]->eat();
    if (--bagList[index].first <= 0)
        bagList.erase(index);
}

void Bag::putItemToStoreHouse(int idx) {
    if (bagList.count(idx) <= 0) {
        globalVar::screen->printMapMes("你的背包沒有這件裝備!!!");
        globalVar::screen->printMapMes(" ");
        return;
    }
    if (idx == weapon || idx == chestplate || idx == pants) {
        globalVar::screen->printMapMes("你不能將已經穿上的裝備放入倉庫!!!");
        globalVar::screen->printMapMes(" ");
        return;
    }
    if (storeHouse.size() >= MAXBAGNUM) {
        globalVar::screen->printMapMes("你的倉庫大小不足!!!");
        globalVar::screen->printMapMes(" ");
        return;
    }
    if (objList[idx]->getJobNumber() != "0") {
        globalVar::screen->printMapMes("你不能將職業限定裝備放到倉庫!!!");
        globalVar::screen->printMapMes(" ");
        return;
    }
    storeHouse.insert({idx, bagList[idx]});
    globalVar::screen->printMapMes("已經將【"+objList[idx]->getName()+"】放入倉庫");
    bagList.erase(idx);
}

void Bag::remItemFromStoreHouse(int idx) {
    if (storeHouse.count(idx) <= 0) {
        globalVar::screen->printMapMes("你沒有這件裝備!!!");
        return;
    }
    if (storeHouse.size() <= 0) {
        globalVar::screen->printMapMes("你的倉庫沒有東西!!!");
        return;
    }
    storeHouse.erase(idx);
    globalVar::screen->printMapMes("已從倉庫將"+objList[idx]->getName()+"移除!!!");
}

void Bag::getObjFromStoreHouse(int idx) {
    if (storeHouse.size() <= 0) {
        globalVar::screen->printMapMes("你的倉庫沒有東西!!!");
        return;
    }
    if (storeHouse.count(idx) <= 0) {
        globalVar::screen->printMapMes("你沒有這件裝備!!!");
        return;
    }

    globalVar::screen->printMapMes("已從倉庫將"+objList[idx]->getName()+"放入背包!!!");
    bagList.insert({idx, storeHouse[idx]});
    storeHouse.erase(idx);
}

void Bag::showStoreHouse() const {
    if (storeHouse.size() <= 0) {
        globalVar::screen->printMapMes("倉庫是空的!!!");
        globalVar::screen->printMapMes(" ");
        return;
    }
    globalVar::screen->printMapMes("倉庫格子數量: " + std::to_string(storeHouse.size()) + "/" + std::to_string(MAXBAGNUM));
    std::string name, space;
    for (const auto& s : storeHouse) {
        name = objList[s.first]->getName();
        int n = 22 - name.size() / 3 * 2;
        globalVar::screen->printMapMes(
            std::to_string(s.first) + ": " + name + std::string(n, ' ') +
            "數量: " + std::to_string(s.second.first) + "  Lv:" +std::to_string(s.second.second) +
            (alreadyWare.count(objList[s.first]->getIdx()) >= 1 ? " 已裝備" : ""));
    }
}

#endif
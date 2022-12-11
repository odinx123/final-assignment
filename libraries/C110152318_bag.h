#ifndef BAG_H
#define BAG_H 1

#include <nlohmann/json.hpp>
#include <vector>

#include "C110152318_object.h"

#define MAXBAGNUM 5
#define EAT 0
#define WARE 1

class Bag {
   private:
    std::string path;
    int objNum = 0;
    int bagNum = 0;
    std::vector<Object*> shopList;
    // {編號, {數量, 物件}}
    std::map<int, std::pair<int, Object*>> bagList;

   public:
    Bag();
    ~Bag();

    inline void showShopList() const;
    inline void showBagList() const;
    // return 0 for ok, 1 for money not enought, 2 for command error, 3 for over size.
    int buy(int n);
    std::string getShopNameByIndex(int idx) { return shopList[idx]->getName(); }
    // return -1 失敗
    int useBagItem(int idx, bool s);
    int getBagNum() const { return bagNum; }
    int getObjNum() const { return objNum; }
    bool isBagCanUse() const { return bagList.size() != 0; }
    void saveBagItem();
    inline void eraseBagItemFromData(int idx);
    void loadBagItem();
    // 0 for eat, 1 for use.
    int getTypeByIdx(int idx) { return shopList[idx]->getType(); }
    inline void throwOut(int idx);
};

Bag::Bag() {
    get_path(path);
    path += "\\player\\user.json";
    globalVar::file_in.open(path);
    globalVar::file_in >> globalVar::jin;
    globalVar::file_in.close();
    // 物品名、加血量、加防禦、加攻擊、效果時間(裝備無ms)、價錢、類型(0是吃的，1是穿的)、編號
    shopList.push_back(objList[0]);
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

Bag::~Bag() {
    for (auto& obj : shopList)
        delete obj;
    for (auto& obj : bagList)
        delete obj.second.second;
}

void Bag::showShopList() const {
    int cnt = 0;
    for (const auto& s : shopList)
        globalVar::screen->printMapMes(
            std::to_string(cnt++) + ": " + s->getName() + " " + std::to_string(s->getCoin()) + "$");
}

void Bag::showBagList() const {
    // int cnt = 0;
    if (bagList.size() == 0) {
        globalVar::screen->printMapMes("Bag is empty!!!");
        return;
    }
    globalVar::screen->printMapMes("背包格子數量: " + std::to_string(bagNum) + "/" + std::to_string(MAXBAGNUM));
    std::string name, space;
    for (const auto& s : bagList) {
        name = s.second.second->getName();
        int n = 22-name.size()/3*2;
        globalVar::screen->printMapMes(
            std::to_string(s.first) + ": " + name+ std::string(n, ' ')+
            "數量: " + std::to_string(s.second.first) +
            (s.second.second->getIdx() == globalVar::user->wareNumber ? " 已裝備" : ""));
    }
}

int Bag::buy(int n) {
    if (n < 0 || n >= shopList.size()) return 2;
    if (bagNum >= MAXBAGNUM) return 3;
    if (globalVar::user->getCoin() < shopList[n]->getCoin()) return 1;
    if (shopList[n]->getJobNumber() != globalVar::user->getCurJob() &&
        shopList[n]->getJobNumber() != "0") return 4;
    globalVar::user->changeCoin(-shopList[n]->getCoin());
    globalVar::user->showCoin();
    if (bagList.count(shopList[n]->getIdx()) > 0)
        ++bagList[shopList[n]->getIdx()].first;
    else {
        bagList.insert({shopList[n]->getIdx(), {1, shopList[n]}});
        ++bagNum;
    }
    return 0;
}

int Bag::useBagItem(int idx, bool s) {
    if (bagList.count(idx) <= 0) {
        globalVar::screen->printMapMes("此裝備不存在!!!");
        globalVar::screen->printMapMes(" ");
        return -1;
    }
    if (bagList[idx].second->getJobNumber() != globalVar::user->getCurJob() &&
        bagList[idx].second->getJobNumber() != "0") {
        globalVar::screen->printMapMes("你的職業無法使用此裝備!!!");
        globalVar::screen->printMapMes(" ");
        return -1;
    }
    auto t = bagList[idx].second->useBagObj(s);
    if (s) {
        if (t && globalVar::user->ware) {
            globalVar::screen->printMapMes("一次只能穿一件裝備!!!");
            return -1;
        }
        if (bagList[idx].first-- > 0 && t) {
            globalVar::user->ware = true;
            globalVar::user->wareNumber = idx;
        } else if (!t && bagList[idx].first == 0) {
            throwOut(idx);
        }
    } else {
        if (t && globalVar::user->wareNumber == idx && globalVar::user->ware) {
            globalVar::user->ware = false;
            globalVar::user->wareNumber = -1;
            ++bagList[idx].first;
        } else {
            globalVar::screen->printMapMes("你沒有穿上" + bagList[idx].second->getName());
        }
    }
    return t;
}

void Bag::saveBagItem() {
    auto& userBag = globalVar::jin["account"][globalVar::user->getCurId()]["bag"];
    userBag = nlohmann::json{
        {"objNum", globalVar::bg->getObjNum()},
        {"bagNum", globalVar::bg->getBagNum()},
    };

    if (globalVar::user->ware) {
        bagList[globalVar::user->wareNumber].second->useBagObj(false);
        ++bagList[globalVar::user->wareNumber].first;
    }

    for (const auto& o : bagList)
        userBag[std::to_string(o.first)] = o.second.first;

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
    auto& userID = globalVar::jin["account"][globalVar::user->getCurId()];
    for (const auto& o : userID["bag"].items()) {
        if (o.key() == "bagNum")
            bagNum = o.value();
        else if (o.key() == "objNum")
            objNum = o.value();
        else if (o.key() == "wareNumber") {
            globalVar::user->ware =  o.value() != -1;
            globalVar::user->wareNumber = o.value();
        } else
            bagList.insert({std::stoi(o.key()), {o.value(), shopList[std::stoi(o.key())]}});
    }
}

void Bag::throwOut(int idx) {
    if (globalVar::user->wareNumber == idx) {
        globalVar::screen->printMapMes("你還穿著這件裝備");
        return;
    }
    bagList.erase(idx);
    eraseBagItemFromData(idx);
    --bagNum;
}

#endif
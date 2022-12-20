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
    // {{編號, Lv}, 數量}
    std::map<std::pair<int, int>, int> storeHouse;
    // {{編號, Lv}, 數量}
    std::map<std::pair<int, int>, int> bagList;
    // {編號, Lv}  武器
    std::pair<int, int> weapon = {-1, 1};
    // {編號, Lv}  胸甲
    std::pair<int, int> chestplate = {-1, 1};
    // {編號, Lv}  褲子
    std::pair<int, int> pants = {-1, 1};

   public:
    Bag();

    inline void showShopList() const;
    inline void showBagList() const;
    // return 0 for ok, 1 for money not enought, 2 for command error, 3 for over size.
    int buy(int n, int lv);
    std::string getShopNameByIndex(int idx) { return shopList[idx]->getName(); }
    // return -1 失敗
    int useBagItem(int idx, int lv, bool s);
    int getBagNum() { return bagList.size(); }
    bool isBagCanUse() { return bagList.size() != 0; }
    void saveBagItem();  // bug
    void loadBagItem();  // bug
    inline int eraseBagItemFromData(int idx, int lv);
    // 0 for eat, 1 for use.
    int getTypeByIdx(int idx) { return (idx < objList.size() && idx >= 0) ? objList[idx]->getType() : -1; }
    inline void throwOut(int idx, int lv);
    inline void addBagItemByIdx(int idx, int lv);
    // 成功穿上裝備回傳0，反之則為-1
    int wareItem(int index, int lv);
    // 成功脫下裝備回傳0，反之則為-1，-2則是裝備不對應
    int deWareItem(int index, int lv);
    inline void eatObj(int index);
    void putItemToStoreHouse(int idx, int lv);
    void remItemFromStoreHouse(int idx, int lv);
    void getObjFromStoreHouse(int idx, int lv);
    void showStoreHouse() const;
    int getStoreNum() { return storeHouse.size(); }
    void setChangeStatus();
    void deSetChangeStatus();
    int getItemNum(int idx, int lv) { return bagList[{idx, lv}]; }
    bool isItemExist(int idx, int lv) const { return bagList.count({idx, lv}) > 0; }
    void levelUP(int idx, int lv, int cost);
    bool isItemWare(int idx, int lv);
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
        name = objList[s.first.first]->getName();
        int n = 22 - name.size() / 3 * 2;
        globalVar::screen->printMapMes(
            std::to_string(s.first.first) + ": " + name + std::string(n, ' ') +
            "數量: " + std::to_string(s.second) +
            ((objList[s.first.first]->getType()) ? ("  Lv:" + std::to_string(s.first.second) +
                                                    ((s.first == weapon || s.first == chestplate || s.first == pants) ? " 已裝備" : ""))
                                                 : " "));
    }
}

int Bag::buy(int n, int lv) {
    if (n < 0 || n >= shopList.size()) return 2;
    if (bagList.size() >= MAXBAGNUM && bagList.count({n, lv}) <= 0) return 3;
    if (globalVar::user->getCoin() < shopList[n]->getCoin()) return 1;
    if (shopList[n]->getJobNumber() != globalVar::user->getCurJob() &&
        shopList[n]->getJobNumber() != "0") return 4;
    globalVar::user->changeCoin(-shopList[n]->getCoin());
    globalVar::user->showCoin();
    if (bagList.count(std::make_pair(shopList[n]->getIdx(), 1)) > 0) {
        auto index = std::make_pair(shopList[n]->getIdx(), 1);
        ++bagList[index];
    } else
        bagList[{shopList[n]->getIdx(), 1}] = 1;
    return 0;
}

int Bag::useBagItem(int idx, int lv, bool s) {  // todo
    if (bagList.count({idx, lv}) <= 0) {
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
            state = wareItem(idx, lv);
            if (state == -1) {
                // 要先脫裝在穿
                // globalVar::screen->printMapMes("這位部位已經有裝備了!!!");

                // 穿上裝備自動替換 /*
                bool set = weapon.first == 13 && chestplate.first == 14 && pants.first == 15;
                globalVar::screen->printMapMes("裝備已被替換掉!!!");
                switch (objList[idx]->getType()) {  // 判斷要脫下哪個裝備(傳進來的穿上的不一定一樣)
                    case ARMS:
                        deWareItem(weapon.first, weapon.second);
                        break;
                    case BODY:
                        deWareItem(chestplate.first, chestplate.second);
                        break;
                    case LEG:
                        deWareItem(pants.first, pants.second);
                        break;
                }
                if (set)                  // 如果脫下裝備前是套裝效果
                    deSetChangeStatus();  // 清除套裝效果(因為要先脫下再穿)
                wareItem(idx, lv);        // 穿上
                setChangeStatus();        // 穿上後如果是套裝效果
                // 穿上裝備自動替換 */
            } else if (weapon.first == 13 && chestplate.first == 14 && pants.first == 15) {
                setChangeStatus();  // 清泉套裝效果
            }
            globalVar::screen->printMapMes(" ");
        } else {
            bool set = weapon.first == 13 && chestplate.first == 14 && pants.first == 15;
            state = deWareItem(idx, lv);
            if (state == -1)
                globalVar::screen->printMapMes("這個部位沒有穿上裝備!!!");
            else if (state == -2)
                globalVar::screen->printMapMes("你沒有穿上【" + objList[idx]->getName() + "】!!!");
            else if (set) {
                deSetChangeStatus();  // 取消套裝效果
            }
            globalVar::screen->printMapMes(" ");
        }
    }

    return state;
}

void Bag::saveBagItem() {
    globalVar::user->saveData();
    auto& userBag = globalVar::jin["account"][globalVar::user->getCurId()]["job"][globalVar::user->getCurJob()]["bag"];
    auto& store = globalVar::jin["account"][globalVar::user->getCurId()]["store"];

    // 脫裝備
    // if (weapon.first != -1)
    //     deWareItem(weapon.first, weapon.second);
    // if (chestplate.first != -1)
    //     deWareItem(chestplate.first, chestplate.second);
    // if (pants.first != -1)
    //     deWareItem(pants.first, pants.second);

    for (const auto& o : bagList) {  // 每次都會有 {pair, lv} 的key代表一個裝備
        std::string number = std::to_string(o.first.first);
        std::string lv = std::to_string(o.first.second);
        bool ware = false;
        if (o.first == weapon || o.first == chestplate || o.first == pants)
            ware = true;
        // 跟map一樣json在key的地方加上新的key會直接新增，若是寫在後面會直接被覆蓋
        userBag[number][lv] = nlohmann::json{// lv底下還要有【數量跟ware】
                                             {"ware", ware},
                                             {"number", o.second}};
    }
    for (const auto& o : storeHouse) {
        std::string number = std::to_string(o.first.first);
        std::string lv = std::to_string(o.first.second);
        store[number][lv] = o.second;  // lv底下只要有數量就可以
    }
    // store[std::to_string(o.first.first)][std::to_string(o.first.second)] = o.second;

    globalVar::file_out.open(path);
    globalVar::file_out << globalVar::jin.dump(4) << std::endl;
    globalVar::file_out.close();
}

void Bag::remItemFromStoreHouse(int idx, int lv) {
    std::string ID = globalVar::user->getCurId();
    auto& store = globalVar::jin["account"][ID]["store"];
    if (store.count(std::to_string(idx)) > 0 &&
        store[std::to_string(idx)].count(std::to_string(lv)) > 0)
        store[std::to_string(idx)].erase(std::to_string(lv));  // 刪除idx中的lv等
    if (store[std::to_string(idx)].is_null())                  // 如果刪除後idx裡面沒有value就刪除
        store.erase(std::to_string(idx));
    if (storeHouse.count(std::make_pair(idx, lv)) > 0)
        storeHouse.erase({idx, lv});
    globalVar::screen->printMapMes("已經將【" + objList[idx]->getName() + "】移除!!!");
    globalVar::screen->printMapMes(" ");
}

int Bag::eraseBagItemFromData(int idx, int lv) {
    std::string ID = globalVar::user->getCurId();
    std::string job = globalVar::user->getCurJob();
    auto& userBag = globalVar::jin["account"][ID]["job"][job]["bag"];
    if (userBag.count(std::to_string(idx)) > 0 &&
        userBag[std::to_string(idx)].count(std::to_string(lv)) > 0)
        userBag[std::to_string(idx)].erase(std::to_string(lv));  // 刪除idx中的lv等
    if (userBag[std::to_string(idx)].is_null())                  // 如果刪除後idx裡面沒有value就刪除
        userBag.erase(std::to_string(idx));
    if (bagList.count(std::make_pair(idx, lv)) > 0) {
        bagList.erase({idx, lv});
        return 0;
    }
    return 1;
}

// todo  chagne job ware equip.
void Bag::loadBagItem() {
    std::string id = globalVar::user->getCurId();
    std::string job = globalVar::user->getCurJob();
    auto& userID = globalVar::jin["account"][id]["job"][job];
    auto& userStore = globalVar::jin["account"][id]["store"];
    for (const auto& o : userID["bag"].items()) {  // 把json的資料讀進bagList
        // o.key()是物品編號
        // value是Lv的json
        int number = std::stoi(o.key());
        for (const auto& oj : o.value().items()) {  // 走訪他的value
            // oj.key()是lv
            // oj.value()是有num跟ware的json
            int lv = std::stoi(oj.key());
            int num = oj.value()["number"];
            bool ware = oj.value()["ware"];

            bagList[{number, lv}] = num;  // 先放進背包
            if (ware) {
                auto kkey = std::make_pair(number, lv);
                int type = objList[number]->getType();
                if (type == ARMS) {
                    weapon = kkey;
                } else if (type == BODY) {
                    chestplate = kkey;
                } else if (type == LEG) {
                    pants = kkey;
                }
            }
        }
    }
    globalVar::set = weapon.first == 13 && chestplate.first == 14 && pants.first == 15;
    if (globalVar::set)
        globalVar::user->incCritiCalRate(0.2);
    for (const auto& o : userStore.items())  // 把json的資料讀進storeHouse
        for (const auto& oj : o.value().items())
            storeHouse[{std::stoi(o.key()), std::stoi(oj.key())}] = oj.value();
}

void Bag::throwOut(int idx, int lv) {
    auto key = std::make_pair(idx, lv);
    if (weapon == key || pants == key || chestplate == key) {
        globalVar::screen->printMapMes("你還穿著這件裝備!!!");
        globalVar::screen->printMapMes(" ");
        return;
    }
    if (bagList.count({idx, lv}) <= 0) {
        globalVar::screen->printMapMes("你沒有這件裝備!!!");
        globalVar::screen->printMapMes(" ");
        return;
    }
    globalVar::screen->printMapMes("已丟棄【" + objList[idx]->getName() + "】!!!");
    bagList.erase({idx, lv});
    eraseBagItemFromData(idx, lv);
}

void Bag::addBagItemByIdx(int idx, int lv) {
    ++bagList[{idx, lv}];
}

int Bag::wareItem(int index, int lv) {
    switch (objList[index]->getType()) {
        case ARMS:
            if (weapon.first != -1)
                return -1;
            weapon = {index, lv};
            break;
        case BODY:
            if (chestplate.first != -1)
                return -1;
            chestplate = {index, lv};
            break;
        case LEG:
            if (pants.first != -1)
                return -1;
            pants = {index, lv};
            break;
    }
    --bagList[{index, lv}];
    objList[index]->use(lv);
    return 0;
}

int Bag::deWareItem(int index, int lv) {
    switch (objList[index]->getType()) {
        case ARMS:
            if (weapon.first == -1)  // todo
                return -1;
            else if (weapon != std::make_pair(index, lv))
                return -2;
            weapon.first = -1;
            break;
        case BODY:
            if (chestplate.first == -1)
                return -1;
            else if (chestplate != std::make_pair(index, lv))
                return -2;
            chestplate.first = -1;
            break;
        case LEG:
            if (pants.first == -1)
                return -1;
            else if (pants != std::make_pair(index, lv))
                return -2;
            pants.first = -1;
            break;
    }
    ++bagList[{index, lv}];
    objList[index]->deuse(lv);
    return 0;
}

void Bag::eatObj(int index) {
    objList[index]->eat();
    if (--bagList[{index, 1}] <= 0)
        bagList.erase({index, 1});
}

void Bag::putItemToStoreHouse(int idx, int lv) {
    if (bagList.count({idx, lv}) <= 0) {
        globalVar::screen->printMapMes("你的背包沒有這件裝備!!!");
        globalVar::screen->printMapMes(" ");
        return;
    }
    auto judge = std::make_pair(idx, lv);
    if (judge == weapon || judge == chestplate || judge == pants) {
        globalVar::screen->printMapMes("你不能將已經穿上的裝備放入倉庫!!!");
        globalVar::screen->printMapMes(" ");
        return;
    }
    if (storeHouse.size() >= MAXBAGNUM && storeHouse.count({idx, lv}) <= 0) {
        globalVar::screen->printMapMes("你的倉庫大小不足!!!");
        globalVar::screen->printMapMes(" ");
        return;
    }
    if (objList[idx]->getJobNumber() != "0") {
        globalVar::screen->printMapMes("你不能將職業限定裝備放到倉庫!!!");
        globalVar::screen->printMapMes(" ");
        return;
    }
    ++storeHouse[{idx, lv}];
    bagList.erase({idx, lv});
    globalVar::screen->printMapMes("已經將【" + objList[idx]->getName() + "】放入倉庫");
}

void Bag::getObjFromStoreHouse(int idx, int lv) {
    if (storeHouse.size() <= 0) {
        globalVar::screen->printMapMes("你的倉庫沒有東西!!!");
        return;
    }
    if (storeHouse.count({idx, lv}) <= 0) {
        globalVar::screen->printMapMes("你沒有這件裝備!!!");
        return;
    }
    if (bagList.size() >= MAXBAGNUM && bagList.count({idx, lv}) <= 0) {
        globalVar::screen->printMapMes("你的背包大小不足!!!");
        return;
    }

    globalVar::screen->printMapMes("已從倉庫將" + objList[idx]->getName() + "放入背包!!!");
    ++bagList[{idx, lv}];
    remItemFromStoreHouse(idx, lv);
}

void Bag::showStoreHouse() const {
    if (storeHouse.size() <= 0) {
        globalVar::screen->printMapMes("倉庫是空的!!!");
        globalVar::screen->printMapMes(" ");
        return;
    }
    globalVar::screen->printMapMes("倉庫格子數量: " + std::to_string(storeHouse.size()) + "/" + std::to_string(MAXBAGNUM));
    std::string name;
    for (const auto& s : storeHouse) {
        name = objList[s.first.first]->getName();
        int n = 22 - name.size() / 3 * 2;
        globalVar::screen->printMapMes(
            std::to_string(s.first.first) + ": " + name + std::string(n, ' ') +
            "數量: " + std::to_string(s.second) +
            ((objList[s.first.first]->getType()) ? ("  Lv:" + std::to_string(s.first.second) +
                                                    ((s.first == weapon || s.first == chestplate || s.first == pants) ? " 已裝備" : ""))
                                                 : " "));
    }
}

void Bag::setChangeStatus() {
    globalVar::set = weapon.first == 13 && chestplate.first == 14 && pants.first == 15;
    if (globalVar::set) {  // 清全效果
        globalVar::screen->printMapMes("觸發套裝效果【清泉之力】");
        globalVar::screen->printMapMes("<HP: 1000, DF: 5, AP: 500>，爆擊率上升20%!!!");
        globalVar::user->jb->chCurAP(500);
        globalVar::user->jb->chCurDF(5);
        globalVar::user->jb->chCurHP(1000);
        globalVar::user->incCritiCalRate(0.2);
    }
}

void Bag::deSetChangeStatus() {
    globalVar::set = false;
    globalVar::screen->printMapMes("套裝效果消失!!!");
    globalVar::screen->printMapMes("<HP: -500, DF: -5, AP: -500>，爆擊率-20%!!!");
    globalVar::user->jb->chCurAP(-500);
    globalVar::user->jb->chCurDF(-5);
    globalVar::user->jb->chCurHP(-1000);
    globalVar::user->decCritiCalRate(0.2);
}

void Bag::levelUP(int idx, int lv, int cost) {
    auto kkey = std::make_pair(idx, lv);
    if (kkey == weapon || kkey == chestplate || kkey == pants) {  // 要先脫裝備
        globalVar::screen->printMapMes("請先脫下裝備再進行升級!!!");
        return;
    }
    if (bagList.size() >= MAXBAGNUM && bagList.count({idx, lv + 1}) <= 0 && bagList[kkey] > 1) {  // 升級後沒有位置且背包已達上限
        globalVar::screen->printMapMes("背包大小不足!!!");
        return;
    }
    if (lv < 5) {
        globalVar::screen->printMapMes(
            "成功將" + objList[idx]->getName() + "從 Lv:" + std::to_string(lv) +
            "強化到 Lv:" + std::to_string(lv + 1));
        if (--bagList[{idx, lv}] <= 0) {  // 如果升級後原本裝備不存在就刪除
            eraseBagItemFromData(idx, lv);
        }
        ++bagList[{idx, lv + 1}];  // 把升級後的裝備加一
        globalVar::user->changeCoin(-cost);
    } else
        globalVar::screen->printMapMes("等級已達上限!!!");
}

bool Bag::isItemWare(int idx, int lv) {
    auto kkey = std::make_pair(idx, lv);
    return (kkey == weapon || kkey == chestplate || kkey == pants);
}

#endif
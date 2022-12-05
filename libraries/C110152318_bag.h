#ifndef BAG_H
#define BAG_H 1

#include <vector>
#include "C110152318_object.h"

class Bag {
   private:
    std::vector<Object*> shopList;
    std::vector<Object*> bagList;

   public:
    Bag(/* args */);
    ~Bag();

    inline void showShopList() const;
    inline void showBagList() const;
    void buy(int n);
};


Bag::Bag(/* args */) {
    // 物品名、加血量、加防禦、加攻擊、效果時間(裝備無ms)、價錢
    shopList.push_back(new Object("發霉的金蘋果", 10, 10, 10, 10000, 100));
    shopList.push_back(new Object("木劍", 10, 0, 0, 0, 100));
    shopList.push_back(new Object("鐵劍", 10, 10, 10, 0, 1000));
    shopList.push_back(new Object("火腿", 10, 10, 10, 0, 10));
}

Bag::~Bag() {
    for (auto& obj : shopList)
        delete obj;
    for (auto& obj : bagList)
        delete obj;
}

void Bag::showShopList() const {
    int cnt = 0;
    for (const auto& s : shopList)
        globalVar::screen->printMapMes(
            std::to_string(cnt++)+": "+s->getName()+" "+std::to_string(s->getCoin())+"$");
}

void Bag::showBagList() const {
    int cnt = 0;
    if (bagList.size() == 0) {
        globalVar::screen->printMapMes("Bag is empty!!!");
        return;
    }
    for (const auto& s : bagList)
        globalVar::screen->printMapMes(std::to_string(cnt++)+": "+s->getName());
}

void Bag::buy(int n) {
    if (n < 0 || n >= shopList.size()) return;
    bagList.emplace_back(shopList[n]);
}

#endif
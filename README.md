# 期末專題RPG	說明文件

[toc]

# I. 地圖功能 <img src="https://github.com/odinx123/final-assignment/blob/main/img/img3.png" width="60">

![](https://github.com/odinx123/final-assignment/blob/main/img/img4.png)

首先是遊戲版面配置是分成上面跟下面，上面都是腳色資訊，下面則是遊戲時的訊息。

地圖設計的部分，會在地圖右上角顯示小地圖，小地圖會標記玩家當前位置，同時左上角也會顯示你現在位置，可以使用上下左右\(方向鍵\) w\,a\,s\,d，也可以使用move指令來進行移動。

如果要打指令需要按下/才能打指令\(移動時不需要\)。

![](https://github.com/odinx123/final-assignment/blob/main/img/img5.png)

# 商店功能 <img src="https://github.com/odinx123/final-assignment/blob/main/img/img6.png" width="60">

<!-- 
![](img%5C%E6%88%91%E7%9A%84%E5%8A%9F%E8%83%BD%E8%AA%AA%E6%98%8E%E6%96%87%E4%BB%B66.png) -->

![](https://github.com/odinx123/final-assignment/blob/main/img/img7.png)

* 遊戲設計可以在【超級商城】買東西，在其他地方無法成功購買物品，同時超級商城也是全地圖唯一沒有怪物的地方，而在超級商城可以使用shop指令來購買東西，也可以使用sell來賣東西，並且之後在背包檢視。
    * 如果使用sell賣東西的話，價錢是根據Lv和數量來決定的。

![](https://github.com/odinx123/final-assignment/blob/main/img/img8.png)

<!-- ![](img%5C%E6%88%91%E7%9A%84%E5%8A%9F%E8%83%BD%E8%AA%AA%E6%98%8E%E6%96%87%E4%BB%B69.png) -->

# II. 怪物功能 <img src="https://github.com/odinx123/final-assignment/blob/main/img/img9.png" width="50">

每次移動地圖或是擊殺一定數量的怪物都會生成新的一批新的怪物，玩家可以使用fight指令選擇跟指定怪物對打，並且當擊殺一定數量的怪物的時候，會繼續生成怪物，每隻怪物都有各自機率生成，有些怪物有可能傳說裝備。

並且每次生成怪物都會顯示怪物資訊，包括血量防禦攻擊等等。

也可以使用ls的指令來顯示怪物資訊。

# 戰鬥資訊 <img src="https://github.com/odinx123/final-assignment/blob/main/img/img11.png" width="50">

![](https://github.com/odinx123/final-assignment/blob/main/img/img10.png)

<!-- ![](img%5C%E6%88%91%E7%9A%84%E5%8A%9F%E8%83%BD%E8%AA%AA%E6%98%8E%E6%96%87%E4%BB%B611.png) -->

玩家可以使用【fight 怪物編號】來跟指定怪物對打，在戰鬥時玩家可以選擇釋放技能\(按空白鍵，需消耗技能點\)，也可以選擇逃跑\(ESC鍵\)，會記錄下總共擊殺多少怪物。

![](https://github.com/odinx123/final-assignment/blob/main/img/img12.png)

# III. 角色/物品功能

![](https://github.com/odinx123/final-assignment/blob/main/img/img13.png)

玩家可以在商店購買裝備或補血、加防禦、攻擊等等的物品。

有些傳說裝備能力較強，只能在打特定怪物時才會掉落。

玩家可以穿在三個部位的裝備分別為【武器】、【胸甲】、【護腿】，玩家可以先脫下在穿上，也可以直接穿上\(直接穿上會自動幫玩家脫下原先部位的裝備。

所有指令為\(bag use、bag deuse\)

丟棄物品可以使用throw

# 屬性能力值

![](https://github.com/odinx123/final-assignment/blob/main/img/img14.png)

玩家可以使用attribute來查看剩下多少【屬性點數】可以用來升級，並且可以查玩家有加多少點數在各個屬性，後面是升級一次加多少能力，可以一次加很多屬性能力。

玩家升級會影響爆擊傷害，等級越高爆擊傷害加成越高\(也受到職業影響\)。

玩家爆擊率是則是需要套裝加成。

爆擊方面可以用showCRT查看。

![](https://github.com/odinx123/final-assignment/blob/main/img/img15.png)

# 等級系統 <img src="https://github.com/odinx123/final-assignment/blob/main/img/img16.png" width="50">

<!-- ![](img%5C%E6%88%91%E7%9A%84%E5%8A%9F%E8%83%BD%E8%AA%AA%E6%98%8E%E6%96%87%E4%BB%B616.png) -->

![](https://github.com/odinx123/final-assignment/blob/main/img/img17.png)

玩家可以藉由打怪獲得經驗值，每隻怪物的經驗都不相同，玩家最高可升級到100等，每次升級會根據不同職業有不同傾向不同能力加成方向，並且會獲得屬性點數、技能點數、技能升級點數等等。

玩家可以在上半部分也就是資訊欄的部份看到玩家的血量攻擊防禦等等，也可以看到擊殺數、技能點、金錢等等，玩家也可以使用showCRT來查看爆擊倍率和爆擊機率。

玩家除了

# 人物狀態欄 <img src="https://github.com/odinx123/final-assignment/blob/main/img/img18.png" width="50">

<!-- ![](img%5C%E6%88%91%E7%9A%84%E5%8A%9F%E8%83%BD%E8%AA%AA%E6%98%8E%E6%96%87%E4%BB%B618.png) -->

玩家除了可以在資訊處看到人物狀態外，要升級技能或查看技能可以使用showSkill或是skill。

![](https://github.com/odinx123/final-assignment/blob/main/img/img19.png)

![](https://github.com/odinx123/final-assignment/blob/main/img/img20.png)

# 裝備加成

![](https://github.com/odinx123/final-assignment/blob/main/img/img21.png)

玩家如果穿上三個部位的全套套裝就會觸發套裝效果，當然如果脫下來就會消失。

遊戲中的套裝無法藉由商店獲得，只能打特殊Bose才能獲得。

![](https://github.com/odinx123/final-assignment/blob/main/img/img22.png)

![](https://github.com/odinx123/final-assignment/blob/main/img/img23.png)

![](https://github.com/odinx123/final-assignment/blob/main/img/img24.png)

<!-- ![](img%5C%E6%88%91%E7%9A%84%E5%8A%9F%E8%83%BD%E8%AA%AA%E6%98%8E%E6%96%87%E4%BB%B625.png) -->

# IV. 點數系統功能 <img src="https://github.com/odinx123/final-assignment/blob/main/img/img25.png" width="50">

在遊戲中有金錢系統，可以在商店中購買各種商品，像是裝備、藥水、食物等等，金錢可以在腳色資訊那邊看到。

屬性點數可以在升級時候獲得，每個等級只能獲得一點屬性點數，可以拿來強化腳色但不會影響腳色升級的加強，所以可以累積一次升級。

技能升級點數可以用來升級技能，玩家可輸入指令showSkill來查看技能等級和剩餘點數。

![](https://github.com/odinx123/final-assignment/blob/main/img/img26.png)

# V. 職業功能

遊戲中可選擇四個職業分別是戰士、法師、刺客、坦克，各自有各自的職業特性跟技能跟裝備\(商店有賣\)，像是刺客有較高的傷害等等。

每個職業的技能都會在5級之後獲得。

![](https://github.com/odinx123/final-assignment/blob/main/img/img27.png)

# VI. 技能功能 <img src="https://github.com/odinx123/final-assignment/blob/main/img/img28.png" width="50">

<!-- ![](img%5C%E6%88%91%E7%9A%84%E5%8A%9F%E8%83%BD%E8%AA%AA%E6%98%8E%E6%96%87%E4%BB%B628.png) -->

![](https://github.com/odinx123/final-assignment/blob/main/img/img29.png)

遊戲可以使用showSkill來查看玩家技能效果和技能有關的資訊，玩家可以升級到5等就可以獲得技能，之後每次升級都可以獲得【技能升級點】，而【技能點】是在使用技能時候會消耗，每次升級後技能會得到加強，但是消耗也會變大。

而技能最高可以升級到6等，可以使用skill升級技能。

![](https://github.com/odinx123/final-assignment/blob/main/img/img30.png)

# VII.強化系統

遊戲可以讓玩家使用屬性點數來強化角色，玩家可以再生及時候獲得。

玩家亦可以升級裝備，使用或升級裝備時如果沒有打上裝備等級，就代表1等，如果打上等級則可選擇該等級的裝備。

![](https://github.com/odinx123/final-assignment/blob/main/img/img31.png)

# 加持道具<img src="https://github.com/odinx123/final-assignment/blob/main/img/img32.png" width="70">

<!-- ![](img%5C%E6%88%91%E7%9A%84%E5%8A%9F%E8%83%BD%E8%AA%AA%E6%98%8E%E6%96%87%E4%BB%B632.png) -->

玩家可以打怪來獲得金蘋果\(不一定會掉落\)，使用後在過數秒後會消失，而補的血量則是固定。

玩家亦可以在商店購買各類補品，有補血家防禦、攻擊等等的道具。

![](https://github.com/odinx123/final-assignment/blob/main/img/img33.png)

![](https://github.com/odinx123/final-assignment/blob/main/img/img34.png)

![](https://github.com/odinx123/final-assignment/blob/main/img/img35.png)

# VIII. 帳號功能 <img src="https://github.com/odinx123/final-assignment/blob/main/img/img41.png" width="50">

![](https://github.com/odinx123/final-assignment/blob/main/img/img36.png)

玩家在登入時可以創建帳號登入遊戲，最多可創建三個帳號，如果已經滿了也可以選擇刪除帳號，使用帳號登入可獲得與之前遊玩一樣的狀態與裝備等等。

如果玩家連續輸入三次錯誤密碼會讓玩家重設密碼\(需輸入兩次相同才可以改成功\)。

玩家可以使用logout來登入不同帳號或是切換職業。

![](https://github.com/odinx123/final-assignment/blob/main/img/img37.png)

![](https://github.com/odinx123/final-assignment/blob/main/img/img38.png)

![](https://github.com/odinx123/final-assignment/blob/main/img/img39.png)

![](https://github.com/odinx123/final-assignment/blob/main/img/img40.png)

<!-- ![](img%5C%E6%88%91%E7%9A%84%E5%8A%9F%E8%83%BD%E8%AA%AA%E6%98%8E%E6%96%87%E4%BB%B641.png) -->

# 倉庫系統 <img src="https://github.com/odinx123/final-assignment/blob/main/img/img42.png" width="50">

<!-- ![](img%5C%E6%88%91%E7%9A%84%E5%8A%9F%E8%83%BD%E8%AA%AA%E6%98%8E%E6%96%87%E4%BB%B642.png) -->

玩家將物品存入倉庫並在另外一個職業拿出來，並且可以在從倉庫移除物品。

放入倉庫、取出、刪除和查看的指令分別為store put、store get、store remove、store show。

玩家不能將職業限定的裝備放入倉庫，也不能將已經穿上的裝備放入昌庫。

![](https://github.com/odinx123/final-assignment/blob/main/img/img43.png)

# IX. 設計其他功能

玩家可以使用指令music play、music stop 來播放或暫停背景音樂。

玩家可以直接將視窗關閉而不會遺失資料。

綁定icon到執行exe上面。

以及要因應聖誕節的情景，遊戲還添加了下雪的背景。

![](https://github.com/odinx123/final-assignment/blob/main/img/img44.png)

![](https://github.com/odinx123/final-assignment/blob/main/img/img45.png)


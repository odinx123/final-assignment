#ifndef MAP_H
#define MAP_H 1

#include <windows.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "function.h"
#include "global.h"

#define WEST "west"
#define EAST "east"
#define NORTH "north"
#define SOUTH "south"

typedef std::unordered_map<std::string, bool> uodMap_sb;
typedef std::unordered_map<std::string, std::string> uodMap_ss;
typedef std::unordered_map<int, std::string> uodMap_is;

class Map {
   private:
    HANDLE stdBuf;
    SHORT height;
    SHORT heightPos;
    nlohmann::json info;
    std::string path;
    std::string curCity;
    COORD northPos;
    COORD westPos;
    std::string northCity;
    std::string westCity;
    uodMap_ss mapDict;
    uodMap_is mapDictI2E;
    uodMap_is mapDictI2C;
    std::string tempCity;
    std::unordered_set<std::string> fileList;

   public:
    Map() = default;
    Map(SHORT Height);

    void loadMap(const std::string& name);
    inline void setStdCursorPos(SHORT x, SHORT y) const;
    inline void setCursorVisible(bool state) const;
    inline SHORT getConsoleWidth() const;
    inline SHORT getConsoleHeight() const;
    inline COORD getCursorPos() const;
    void setConsoleName(const std::wstring& name) const;
    void setColor(WORD color = 7) const;
    void clearMap() const;
    // EAST, WEST, NORTH, SOUTH
    inline void move(const std::string& dir);
    void showMap(SHORT x, SHORT y);
    void setMes(const std::string& mes, SHORT x, SHORT y);
    void clearMes(SHORT x, SHORT y, SHORT size_);
    void setAllCity();
    inline void setInfoCursorPos(SHORT x, SHORT y);
    std::string getEngCityName(int i) { return mapDictI2E[i]; }
    std::string getChsCityName(int i) { return mapDictI2C[i]; }
    int getCityNum() const { return mapDictI2E.size(); }

   private:
    void testAllCity(uodMap_sb& hash_, const std::string& name, SHORT w, SHORT h);
    void showMap(uodMap_sb& hash_, std::string name, SHORT x, SHORT y);
};

Map::Map(SHORT Height) {
    height = Height;
    stdBuf = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setCursorVisible(false);
    SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 300, 35, 0, 0, SWP_NOSIZE);

    get_path(path);
    path += "\\map";
    getFiles(path, fileList);

    globalVar::file_in.open(path + "\\information.json");
    globalVar::file_in >> info;
    globalVar::file_in.close();


    setAllCity();
    loadMap("map01");
    showMap(90, 1);
}

void Map::loadMap(const std::string& name) {
    std::string curMapFile = path + "\\" + name + ".txt";
    auto width = getConsoleWidth();

    setStdCursorPos(0, height);
    std::cout << std::string(width, '=');

    if (fileList.count(curMapFile) <= 0) {
        setColor(4);
        setMes("This map does not exist.", 0, 0);
        setColor();
        setStdCursorPos(0, 0);
        return;
    }
    curCity = name;
    clearMap();
    clearMes(0, 0, 30);
    clearMes(0, 1, 30);
    setStdCursorPos(0, height + 2);

    std::string temp;
    globalVar::file_in.open(curMapFile);
    getline(globalVar::file_in, temp);

    if (temp.size() <= 18) {
        WriteConsoleW(stdBuf, L"你現在位於: ", 8, nullptr, nullptr);
        std::cout << temp;
    }
    else {
        WriteConsoleW(stdBuf, L"你現在位於: ", 8, nullptr, nullptr);
        std::cout << temp.substr(0, 18);
    }
    showMap(90, 1);

    setStdCursorPos(0, 0);
    for (int i = 1; i < height && !globalVar::file_in.eof(); ++i) {
        getline(globalVar::file_in, temp);
        std::cout << temp << std::endl;
    }
    heightPos = getCursorPos().Y+1;
    globalVar::file_in.close();
    setStdCursorPos(0, 0);
}

void Map::setStdCursorPos(SHORT x, SHORT y) const {
    SetConsoleCursorPosition(stdBuf, {x, y});
}

void Map::setCursorVisible(bool state) const {
    CONSOLE_CURSOR_INFO cci{1, state};
    SetConsoleCursorInfo(stdBuf, &cci);
}

SHORT Map::getConsoleWidth() const {
    CONSOLE_SCREEN_BUFFER_INFO size_;
    GetConsoleScreenBufferInfo(stdBuf, &size_);
    return size_.dwSize.X;
}

SHORT Map::getConsoleHeight() const {
    CONSOLE_SCREEN_BUFFER_INFO size_;
    GetConsoleScreenBufferInfo(stdBuf, &size_);
    return size_.srWindow.Bottom;
}

void Map::clearMap() const {
    setStdCursorPos(0, 0);
    SHORT width = getConsoleWidth();
    for (SHORT i = 0; i < height; ++i)
        std::cout << std::string(width, ' ');
    setStdCursorPos(0, 0);
}

COORD Map::getCursorPos() const {
    CONSOLE_SCREEN_BUFFER_INFO size_;
    GetConsoleScreenBufferInfo(stdBuf, &size_);
    return size_.dwCursorPosition;
}

void Map::setConsoleName(const std::wstring& name) const {
    SetConsoleTitleW(name.c_str());
}

void Map::setColor(WORD color) const {
    SetConsoleTextAttribute(stdBuf, color);
}

void Map::move(const std::string& dir) {
    loadMap(info.at(curCity).at(dir));
}

void Map::showMap(SHORT x, SHORT y) {
    auto orgPos = getCursorPos();
    uodMap_sb hash_;
    showMap(hash_, westCity, x + 0, y + height + 1 + abs(westPos.Y - northPos.Y) * 2);
    setStdCursorPos(orgPos.X, orgPos.Y);
}

void Map::showMap(uodMap_sb& hash_, std::string name, SHORT x, SHORT y) {
    // Base case
    if (name == "null" || hash_[name])
        return;

    // Recursion relation
    hash_[name] = true;
    setStdCursorPos(x, y);
    if (name == curCity)
        setColor(31);
    std::cout << mapDict[name];
    setColor();

    auto wordWide = isalpha(mapDict[name][0]) ? name.size() : mapDict[name].size()/1.5;
    if (info[name]["west"] != "null" && !hash_[info[name]["west"]]) {
        setStdCursorPos(x - 2, y);
        std::cout << "--";
    }
    if (info[name]["east"] != "null" && !hash_[info[name]["east"]]) {
        std::cout << "--";
    }
    if (info[name]["north"] != "null" && !hash_[info[name]["north"]]) {
        setStdCursorPos(x + wordWide / 2-1, y - 1);
        std::cout << "|";
    }
    if (info[name]["south"] != "null" && !hash_[info[name]["south"]]) {
        setStdCursorPos(x + wordWide / 2-1, y + 1);
        std::cout << "|";
    }
    showMap(hash_, info[name]["west"], x - wordWide - 2, y);
    showMap(hash_, info[name]["east"], x + wordWide + 2, y);
    showMap(hash_, info[name]["north"], x, y - 2);
    showMap(hash_, info[name]["south"], x, y + 2);
}

void Map::setMes(const std::string& mes, SHORT x, SHORT y) {
    auto orgPos = getCursorPos();
    setInfoCursorPos(x, y);
    std::cout << mes;
    setStdCursorPos(orgPos.X, orgPos.Y);
}

void Map::clearMes(SHORT x, SHORT y, SHORT size_) {
    auto orgPos = getCursorPos();
    setStdCursorPos(x, y + height + 1);
    std::cout << std::string(size_, ' ');
    setStdCursorPos(orgPos.X, orgPos.Y);
}

void Map::setAllCity() {
    northPos = {0, 0};
    westPos = {0, 0};
    uodMap_sb hash_;
    std::string name = fileList.begin()->substr(fileList.begin()->find_last_of('\\') + 1);
    name = name.substr(0, name.find_last_of('.'));
    northCity = name;
    westCity = name;
    testAllCity(hash_, name, 0, 0);
}

void Map::testAllCity(uodMap_sb& hash_, const std::string& name, SHORT w, SHORT h) {
    if (name == "null" || hash_[name])
        return;

    static int i = 0;
    globalVar::file_in.open(path + "\\" + name + ".txt");
    globalVar::file_in >> tempCity;
    mapDict[name] = tempCity;
    mapDictI2E[i] = name;
    mapDictI2C[i++] = tempCity;
    globalVar::file_in.close();

    hash_[name] = true;
    if (info[name]["west"] != "null") {
        if (w - 1 < westPos.X) {
            westPos.X = w - 1;
            westPos.Y = h;
            westCity = info[name]["west"];
        }
        testAllCity(hash_, info[name]["west"], w - 1, h);
    }
    if (info[name]["north"] != "null") {
        if (h - 1 < northPos.Y) {
            northPos.X = w;
            northPos.Y = h - 1;
            northCity = info[name]["north"];
        }
        testAllCity(hash_, info[name]["north"], w, h - 1);
    }
    if (info[name]["east"] != "null")
        testAllCity(hash_, info[name]["east"], w + 1, h);
    if (info[name]["south"] != "null")
        testAllCity(hash_, info[name]["south"], w, h + 1);
}

void Map::setInfoCursorPos(SHORT x, SHORT y) {
    setStdCursorPos(x, height + y + 1);
}

#endif
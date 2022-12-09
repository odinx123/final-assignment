#include <stdio.h>
#include <windows.h>
#include <vector>

#include <iostream>
using namespace std;

int main(void) {
    vector<int> arr;
    for (int i = 0; i < 10; ++i)
        arr.emplace_back(i);
    
    for (const auto& i : arr)
        cout << i << " ";
    cout << endl;

    arr.erase(arr.begin() + 4);

    for (const auto& i : arr)
        cout << i << " ";
    cout << endl;

    system("pause");
    return 0;
}
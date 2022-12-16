#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;

class a {
   public:
    void print() {
        cout << "Hello word" << endl;
    }
};

class b : public a {
};

int main() {
    b d;
    d.print();

    system("pause");
    return 0;
}

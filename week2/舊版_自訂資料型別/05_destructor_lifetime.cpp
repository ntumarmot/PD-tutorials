#include <iostream>
#include <string>
using namespace std;

class Trace {
private:
    string name;

public:
    Trace(string objectName) {
        name = objectName;
        cout << "建立 " << name << '\n';
    }

    ~Trace() {
        cout << "結束 " << name << '\n';
    }
};

void study() {
    Trace inFunction("函式內");
    cout << "準備離開函式" << '\n';
}

int main() {
    Trace outer("main 內");
    {
        Trace inner("小區塊內");
        cout << "準備離開小區塊" << '\n';
    }
    study();
    cout << "準備離開 main" << '\n';
}

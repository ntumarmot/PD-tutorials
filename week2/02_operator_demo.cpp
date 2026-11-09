#include <iostream>
using namespace std;

class Box {
private:
    int value;

public:
    Box(int initialValue) {
        value = initialValue;
    }

    bool operator==(const Box& other) const {
        return value == other.value;
    }
};

int main() {
    const Box a(5);
    const Box b(5);
    const Box c(8);

    cout << (a == b) << '\n';           // 1：相等
    cout << a.operator==(b) << '\n';   // 1：同一個成員函式
    cout << (a == c) << '\n';           // 0：不相等
}

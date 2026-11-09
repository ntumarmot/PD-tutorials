#include <iostream>
using namespace std;

class Point {
private:
    int x, y;

public:
    Point(int initialX, int initialY)
        : x(initialX), y(initialY) {}

    int getX() const { return x; }
    int getY() const { return y; }

    void print() const {
        cout << "(" << x << ", " << y << ")" << endl;
    }
};

int main(){
    const Point fixed(3, 4);
    fixed.print();
    fixed.x = 8;
}
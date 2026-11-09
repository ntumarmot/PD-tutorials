#include <cstdlib>
#include <iostream>
using namespace std;

class Vec2 {
private:
    int x;
    int y;

public:
    Vec2(int startX, int startY) : x(startX), y(startY) {}

    int getX() const { return x; }
    int getY() const { return y; }

    void print() const {
        cout << '(' << x << ", " << y << ')';
    }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    int lengthSquared() const {
        return x * x + y * y;
    }

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vec2& other) const {
        return !(*this == other);
    }

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2 operator+(const Vec2& other) const {
        Vec2 result(*this);
        result += other;
        return result;
    }

    int operator[](int index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        cerr << "index 只能是 0 或 1\n";
        exit(1);
    }

    int& operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;
        cerr << "index 只能是 0 或 1\n";
        exit(1);
    }

    Vec2 operator+(int scalar) const {
        return Vec2(x + scalar, y + scalar);
    }
};

Vec2 operator+(int scalar, const Vec2& vector) {
    return vector + scalar;
}

int main() {
    const Vec2 fixed(3, 4);
    cout << "固定向量：";
    fixed.print();
    cout << "，長度平方：" << fixed.lengthSquared() << '\n';

    Vec2 moving(3, 4);
    moving.move(-1, 2);
    cout << "移動後：";
    moving.print();
    cout << "，長度平方：" << moving.lengthSquared() << '\n';

    Vec2 a(1, 2);
    Vec2 b(3, 4);
    cout << "相等：" << (a == b) << "，不相等：" << (a != b) << '\n';

    a += b;
    cout << "a += b：";
    a.print();
    cout << '\n';

    Vec2 self(1, 2);
    self += self;
    cout << "self += self：";
    self.print();
    cout << '\n';

    Vec2 plusA(1, 2);
    Vec2 plusB(3, 4);
    Vec2 sum = plusA + plusB;
    cout << "(1,2) + (3,4)：";
    sum.print();
    cout << '\n';

    sum[0] = 9;
    cout << "改寫 sum[0] 後：";
    sum.print();
    cout << "，fixed[1] = " << fixed[1] << '\n';

    Vec2 scalarBase(2, 5);
    cout << "(2,5) + 3：";
    (scalarBase + 3).print();
    cout << "，3 + (2,5)：";
    (3 + scalarBase).print();
    cout << '\n';
}

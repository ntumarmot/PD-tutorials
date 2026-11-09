#include <cstdlib>
#include <iostream>
using namespace std;

class IntArray {
private:
    int size;
    int* data;

public:
    IntArray(int length) : size(length), data(nullptr) {
        if (length < 0) {
            cerr << "長度不能小於 0\n";
            exit(1);
        }
        data = new int[size]{};
    }

    ~IntArray() {
        delete[] data;
    }

    IntArray(const IntArray& other) : size(other.size), data(new int[other.size]) {
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    IntArray& operator=(const IntArray& other) {
        if (this == &other) {
            return *this;
        }

        int* newData = new int[other.size];
        for (int i = 0; i < other.size; i++) {
            newData[i] = other.data[i];
        }

        delete[] data;
        data = newData;
        size = other.size;
        return *this;
    }

    int& operator[](int index) {
        if (index < 0 || index >= size) {
            cerr << "索引超出範圍\n";
            exit(1);
        }
        return data[index];
    }

    int operator[](int index) const {
        if (index < 0 || index >= size) {
            cerr << "索引超出範圍\n";
            exit(1);
        }
        return data[index];
    }
};

int main() {
    IntArray a(2);
    a[0] = 5;
    a[1] = 7;

    IntArray b(1);
    b[0] = 9;
    b = a;
    a[0] = 42;
    cout << "a[0] = " << a[0] << ", b[0] = " << b[0] << '\n';

    IntArray& sameObject = a;
    a = sameObject; // 執行時仍是自我指定
    cout << "自我指定後 a[0] = " << a[0] << '\n';

    IntArray c(b);
    b[1] = 100;
    cout << "b[1] = " << b[1] << ", c[1] = " << c[1] << '\n';
}

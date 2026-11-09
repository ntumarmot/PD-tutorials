#include <iostream>
#include <string>
using namespace std;

struct Student {
    string name;
    int score;

    void print() {
        cout << name << ": " << score << '\n';
    }
};

int main() {
    Student alice = {"小明", 80};
    Student bob = {"小華", 95};

    alice.print();
    bob.print();
}

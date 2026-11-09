#include <iostream>
#include <string>
using namespace std;

struct Student {
    string name;
    int score;

    void print() {
        cout << name << ": " << score << '\n';
    }

    bool isPass() {
        return score >= 60;
    }
};

Student better(Student a, Student b) {
    if (a.score >= b.score) {
        return a;
    }
    return b;
}

int main() {
    Student alice = {"小明", 59};
    Student bob = {"小華", 60};
    Student chris = {"小安", 100};

    Student winner = better(alice, bob);
    winner.print();
    better(bob, alice).print();
    Student sameScore = {"小樂", 60};
    better(bob, sameScore).print(); // 同分時回傳第一位：小華

    Student students[3] = {alice, bob, chris};
    int passCount = 0;
    for (int i = 0; i < 3; i++) {
        if (students[i].isPass()) {
            students[i].print();
            passCount++;
        }
    }
    cout << "及格人數：" << passCount << '\n';
}

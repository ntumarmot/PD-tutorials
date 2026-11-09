#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    string name;
    int score;

public:
    Student() {
        name = "未命名";
        score = 0;
    }

    Student(string studentName, int initialScore) {
        name = studentName;
        score = 0;
        setScore(initialScore);
    }

    string getName() {
        return name;
    }

    int getScore() {
        return score;
    }

    bool setScore(int newScore) {
        if (newScore < 0 || newScore > 100) {
            return false;
        }
        score = newScore;
        return true;
    }

    void print() {
        cout << name << ": " << score << '\n';
    }
};

Student better(Student a, Student b) {
    if (a.getScore() >= b.getScore()) {
        return a;
    }
    return b;
}

int main() {
    Student unknown;
    Student alice("小明", 80);
    Student bob("小華", 120);
    unknown.print();
    alice.print();
    bob.print();

    cout << "比較勝出：" << better(alice, bob).getName() << '\n';

    int trials[4] = {-1, 0, 100, 101};
    for (int trial : trials) {
        bool changed = alice.setScore(trial);
        cout << "修改成功：" << changed << "，目前分數：" << alice.getScore() << '\n';
    }
}

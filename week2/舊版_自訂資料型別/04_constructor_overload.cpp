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

    void setScore(int newScore) {
        if (newScore >= 0 && newScore <= 100) {
            score = newScore;
        }
    }

    void print() {
        cout << name << ": " << score << '\n';
    }
};

int main() {
    Student unknown;
    Student alice("小明", 80);
    Student bob("小華", 120);

    unknown.print();
    alice.print();
    bob.print();
}

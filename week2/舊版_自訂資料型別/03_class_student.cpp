#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    string name;
    int score;

public:
    Student(string studentName) {
        name = studentName;
        score = 0;
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
    Student alice("小明");
    alice.setScore(80);
    alice.print();

    alice.setScore(120);
    alice.print();
}

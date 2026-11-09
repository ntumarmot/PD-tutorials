#include <iostream>
#include <string>
using namespace std;

void printStudent(string name, int score) {
    cout << name << ": " << score << '\n';
}

int main() {
    string aliceName = "小明";
    int aliceScore = 80;
    string bobName = "小華";
    int bobScore = 95;

    printStudent(aliceName, aliceScore);
    printStudent(bobName, bobScore);
}

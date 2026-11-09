#include <iostream>
#include <string>
using namespace std;

class BankAccount {
private:
    string owner;
    int balance;

public:
    BankAccount(string accountOwner) {
        owner = accountOwner;
        balance = 0;
    }

    bool deposit(int amount) {
        if (amount <= 0) {
            return false;
        }
        balance += amount;
        return true;
    }

    int getBalance() const {
        return balance;
    }

    void print() const {
        cout << owner << " 的餘額：" << balance << '\n';
    }
};

int main() {
    BankAccount account("小明");
    account.deposit(100);
    account.deposit(-20);
    account.print();
    cout << "目前餘額：" << account.getBalance() << '\n';
}

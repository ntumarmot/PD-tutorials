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

    bool withdraw(int amount) {
        if (amount <= 0 || amount > balance) {
            return false;
        }
        balance -= amount;
        return true;
    }

    bool transferTo(BankAccount& other, int amount) {
        if (amount <= 0 || amount > balance) {
            return false;
        }
        balance -= amount;
        other.balance += amount;
        return true;
    }

    void print() {
        cout << owner << " 的餘額：" << balance << '\n';
    }
};

int main() {
    BankAccount account("小明");
    cout << "存入 100：" << account.deposit(100) << '\n';
    account.print();

    cout << "存入 -20：" << account.deposit(-20) << '\n';
    account.print();

    cout << "提領 30：" << account.withdraw(30) << '\n';
    account.print();

    cout << "提領 200：" << account.withdraw(200) << '\n';
    account.print();

    cout << "提領 0：" << account.withdraw(0) << '\n';
    account.print();

    BankAccount friendAccount("小華");
    cout << "轉出 20：" << account.transferTo(friendAccount, 20) << '\n';
    account.print();
    friendAccount.print();
}

#pragma once

#include <string>
#include <vector>

using namespace std;
using Money = long long; // All monetary values are stored in paise.

string formatMoney(Money paise);

struct Transaction {
    string type;
    Money amount;
    Money balanceAfter;
};

class Account {
private:
    int id;
    string owner;
    Money balance;
    vector<Transaction> history;

public:
    Account(int accountId, const string& accountOwner, Money initialBalance = 0);

    int getId() const;
    string getOwner() const;
    Money getBalance() const;
    const vector<Transaction>& getHistory() const;

    bool deposit(Money amount);
    bool canWithdraw(Money amount) const;
    bool withdraw(Money amount);

    void loadTransaction(const Transaction& tx);

    void showBalance() const;
    void showHistory() const;
};

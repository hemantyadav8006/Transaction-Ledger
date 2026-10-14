#include "Account.h"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

string formatMoney(Money paise) {
    return "Rs. " + to_string(paise / 100) + "." +
           (paise % 100 < 10 ? "0" : "") +
           to_string(paise % 100);
}

Account::Account(int accountId, const string& accountOwner, Money initialBalance)
    : id(accountId), owner(accountOwner), balance(initialBalance) {}

int Account::getId() const {
    return id;
}

string Account::getOwner() const {
    return owner;
}

Money Account::getBalance() const {
    return balance;
}

const vector<Transaction>& Account::getHistory() const {
    return history;
}

bool Account::deposit(Money amount) {
    if (amount <= 0 ||
        amount > numeric_limits<Money>::max() - balance) {
        return false;
    }

    balance += amount;
    history.push_back({"Deposit", amount, balance});
    return true;
}

bool Account::canWithdraw(Money amount) const {
    return amount > 0 && amount <= balance;
}

bool Account::withdraw(Money amount) {
    if (!canWithdraw(amount)) {
        return false;
    }

    balance -= amount;
    history.push_back({"Withdrawal", amount, balance});
    return true;
}

void Account::loadTransaction(const Transaction& tx) {
    history.push_back(tx);
}

void Account::showBalance() const {
    cout << "\nAccount ID: " << id
              << "\nOwner: " << owner
              << "\nBalance: " << formatMoney(balance) << '\n';
}

void Account::showHistory() const {
    cout << "\nTransaction history for " << owner << ":\n";

    if (history.empty()) {
        cout << "No transactions yet.\n";
        return;
    }

    for (const Transaction& transaction : history) {
        cout << transaction.type << " | "
                  << formatMoney(transaction.amount)
                  << " | Balance after: "
                  << formatMoney(transaction.balanceAfter) << '\n';
    }
}

#pragma once

#include "Account.h"
#include "Storage.h"

#include <string>
#include <unordered_map>
using namespace std;

class Ledger {
private:
    unordered_map<int, Account> accounts;
    int nextId = 1001;
    Storage storage;

public:
    Ledger();

    int createAccount(const string& owner);
    Account* findAccount(int id);

    bool deposit(int id, Money amount);
    bool withdraw(int id, Money amount);
};

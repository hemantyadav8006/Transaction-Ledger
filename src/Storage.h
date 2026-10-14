#pragma once

#include "Account.h"

#include <string>
#include <unordered_map>
using namespace std;

class Storage {
private:
    string accountsFilePath;
    string transactionsFilePath;

    void ensureDirectory();

public:
    Storage(const string& accountsFile = "data/accounts.csv",
            const string& transactionsFile = "data/transactions.csv");

    bool loadAll(unordered_map<int, Account>& accounts, int& maxId);
    bool appendAccount(const Account& account);
    bool appendTransaction(int accountId, const Transaction& tx);
    bool updateAccounts(const unordered_map<int, Account>& accounts);
};

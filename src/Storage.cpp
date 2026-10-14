#include "Storage.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

namespace {
    bool fileIsEmpty(const string& path) {
        ifstream in(path, ios::ate | ios::binary);
        return !in.is_open() || in.tellg() == 0;
    }
}

Storage::Storage(const string& accountsFile, const string& transactionsFile)
    : accountsFilePath(accountsFile), transactionsFilePath(transactionsFile) {
    ensureDirectory();
}

void Storage::ensureDirectory() {
    MKDIR("data");
}

bool Storage::loadAll(unordered_map<int, Account>& accounts, int& maxId) {
    ensureDirectory();

    // 1. Load Accounts
    ifstream accFile(accountsFilePath);
    if (accFile.is_open()) {
        string line;
        bool isFirstLine = true;

        while (getline(accFile, line)) {
            if (line.empty()) continue;

            // Skip CSV header
            if (isFirstLine) {
                isFirstLine = false;
                if (line.rfind("id,", 0) == 0) continue;
            }

            stringstream ss(line);
            string idStr, owner, balanceStr;

            if (getline(ss, idStr, ',') &&
                getline(ss, owner, ',') &&
                getline(ss, balanceStr, ',')) {
                try {
                    int id = stoi(idStr);
                    Money balance = stoll(balanceStr);

                    accounts.emplace(id, Account(id, owner, balance));
                    maxId = max(maxId, id);
                } catch (const exception& e) {
                    cerr << "Warning: Skipped corrupted account record: " << line << '\n';
                }
            }
        }
        accFile.close();
    }

    // 2. Load Transactions and associate with existing accounts
    ifstream txFile(transactionsFilePath);
    if (txFile.is_open()) {
        string line;
        bool isFirstLine = true;

        while (getline(txFile, line)) {
            if (line.empty()) continue;

            // Skip CSV header
            if (isFirstLine) {
                isFirstLine = false;
                if (line.rfind("accountId,", 0) == 0) continue;
            }

            stringstream ss(line);
            string accIdStr, type, amountStr, balanceAfterStr;

            if (getline(ss, accIdStr, ',') &&
                getline(ss, type, ',') &&
                getline(ss, amountStr, ',') &&
                getline(ss, balanceAfterStr, ',')) {
                try {
                    int accId = stoi(accIdStr);
                    Money amount = stoll(amountStr);
                    Money balanceAfter = stoll(balanceAfterStr);

                    auto it = accounts.find(accId);
                    if (it != accounts.end()) {
                        it->second.loadTransaction({type, amount, balanceAfter});
                    }
                } catch (const exception& e) {
                    cerr << "Warning: Skipped corrupted transaction record: " << line << '\n';
                }
            }
        }
        txFile.close();
    }

    return true;
}

bool Storage::appendAccount(const Account& account) {
    ensureDirectory();
    bool writeHeader = fileIsEmpty(accountsFilePath);

    ofstream out(accountsFilePath, ios::app);
    if (!out.is_open()) {
        cerr << "Error: Unable to open " << accountsFilePath << " for writing.\n";
        return false;
    }

    if (writeHeader) {
        out << "id,owner,balance\n";
    }

    out << account.getId() << ","
        << account.getOwner() << ","
        << account.getBalance() << "\n";

    return true;
}

bool Storage::appendTransaction(int accountId, const Transaction& tx) {
    ensureDirectory();
    bool writeHeader = fileIsEmpty(transactionsFilePath);

    ofstream out(transactionsFilePath, ios::app);
    if (!out.is_open()) {
        cerr << "Error: Unable to open " << transactionsFilePath << " for writing.\n";
        return false;
    }

    if (writeHeader) {
        out << "accountId,type,amount,balanceAfter\n";
    }

    out << accountId << ","
        << tx.type << ","
        << tx.amount << ","
        << tx.balanceAfter << "\n";

    return true;
}

bool Storage::updateAccounts(const unordered_map<int, Account>& accounts) {
    ensureDirectory();

    ofstream out(accountsFilePath, ios::trunc);
    if (!out.is_open()) {
        cerr << "Error: Unable to rewrite " << accountsFilePath << ".\n";
        return false;
    }

    out << "id,owner,balance\n";
    for (const auto& pair : accounts) {
        const Account& acc = pair.second;
        out << acc.getId() << ","
            << acc.getOwner() << ","
            << acc.getBalance() << "\n";
    }

    return true;
}

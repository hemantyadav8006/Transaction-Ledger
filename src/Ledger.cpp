#include "Ledger.h"
using namespace std;

Ledger::Ledger() {
    int maxId = 1000;
    storage.loadAll(accounts, maxId);

    if (maxId >= 1001) {
        nextId = maxId + 1;
    } else {
        nextId = 1001;
    }
}

int Ledger::createAccount(const string& owner) {
    int id = nextId++;
    auto result = accounts.emplace(id, Account(id, owner, 0));

    if (result.second) {
        storage.appendAccount(result.first->second);
    }

    return id;
}

Account* Ledger::findAccount(int id) {
    auto it = accounts.find(id);

    if (it == accounts.end()) {
        return nullptr;
    }

    return &it->second;
}

bool Ledger::deposit(int id, Money amount) {
    Account* acc = findAccount(id);
    if (acc == nullptr) {
        return false;
    }

    if (!acc->deposit(amount)) {
        return false;
    }

    storage.appendTransaction(id, acc->getHistory().back());
    storage.updateAccounts(accounts);
    return true;
}

bool Ledger::withdraw(int id, Money amount) {
    Account* acc = findAccount(id);
    if (acc == nullptr) {
        return false;
    }

    if (!acc->withdraw(amount)) {
        return false;
    }

    storage.appendTransaction(id, acc->getHistory().back());
    storage.updateAccounts(accounts);
    return true;
}

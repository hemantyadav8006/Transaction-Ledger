#include "Ledger.h"

#include <iostream>
#include <limits>
#include <string>

using namespace std;

long long readInteger(const string& prompt) {
    long long value;

    while (true) {
        cout << prompt;

        if (cin >> value) {
            return value;
        }

        cout << "Invalid input. Enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    Ledger ledger;
    int choice;

    while (true) {
        cout << "\n=== Transaction Ledger ===\n"
             << "1. Create account\n"
             << "2. Deposit\n"
             << "3. Withdraw\n"
             << "4. Check balance\n"
             << "5. View transaction history\n"
             << "0. Exit\n";

        choice = static_cast<int>(readInteger("Choose an option: "));

        if (choice == 0) {
            cout << "Thank you for using the transaction ledger." << endl;
            cout << "Goodbye!\n";
            break;
        }

        if (choice == 1) {
            string owner;
            cout << "Enter account holder name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, owner);

            if (owner.empty()) {
                cout << "Name cannot be empty.\n";
                continue;
            }

            int id = ledger.createAccount(owner);
            cout << "Account created. ID: " << id << '\n';
            continue;
        }

        if (choice < 2 || choice > 5) {
            cout << "Invalid menu option.\n";
            continue;
        }

        int id = static_cast<int>(readInteger("Enter account ID: "));
        Account* account = ledger.findAccount(id);

        if (account == nullptr) {
            cout << "Account not found.\n";
            continue;
        }

        if (choice == 2 || choice == 3) {
            long long rupees = readInteger("Enter amount in whole rupees: ");

            if (rupees <= 0 ||
                rupees > numeric_limits<Money>::max() / 100) {
                cout << "Amount must be positive and within range.\n";
                continue;
            }

            Money amount = rupees * 100;
            bool success = choice == 2
                ? ledger.deposit(id, amount)
                : ledger.withdraw(id, amount);

            if (success) {
                cout << "Transaction successful. Balance: "
                     << formatMoney(account->getBalance()) << '\n';
            } else {
                cout << "Transaction rejected. Check the amount "
                     << "or available balance.\n";
            }
        } else if (choice == 4) {
            account->showBalance();
        } else if (choice == 5) {
            account->showHistory();
        }
    }

    return 0;
}

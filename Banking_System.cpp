#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

class Transaction {
public:
    string type;        // "DEPOSIT", "WITHDRAW", "TRANSFER_IN", "TRANSFER_OUT"
    double amount;
    int fromAcc;
    int toAcc;
    double balanceAfter;

    Transaction(string t, double a, int from, int to, double balAfter)
        : type(t), amount(a), fromAcc(from), toAcc(to), balanceAfter(balAfter) {}
};

class Customer {
public:
    int id;
    string name;

    Customer(int id, const string &name) : id(id), name(name) {}
};

class Account {
public:
    int accountNumber;
    int customerId;
    double balance;
    vector<Transaction> history;

    Account(int accNo, int custId, double initial)
        : accountNumber(accNo), customerId(custId), balance(initial) {}

    void addTransaction(const string &type, double amount, int from, int to) {
        history.emplace_back(type, amount, from, to, balance);
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Amount must be positive.\n";
            return;
        }
        balance += amount;
        addTransaction("DEPOSIT", amount, accountNumber, accountNumber);
        cout << "Deposit successful. New balance: " << balance << "\n";
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Amount must be positive.\n";
            return;
        }
        if (amount > balance) {
            cout << "Insufficient balance.\n";
            return;
        }
        balance -= amount;
        addTransaction("WITHDRAW", amount, accountNumber, accountNumber);
        cout << "Withdrawal successful. New balance: " << balance << "\n";
    }

    void transferTo(Account &other, double amount) {
        if (amount <= 0) {
            cout << "Amount must be positive.\n";
            return;
        }
        if (amount > balance) {
            cout << "Insufficient balance.\n";
            return;
        }

        balance -= amount;
        other.balance += amount;

        addTransaction("TRANSFER_OUT", amount, accountNumber, other.accountNumber);
        other.addTransaction("TRANSFER_IN", amount, accountNumber, other.accountNumber);

        cout << "Transfer successful. Your new balance: " << balance << "\n";
    }

    void showInfo(const string &customerName) {
        cout << "Account Number: " << accountNumber << "\n";
        cout << "Customer Name: " << customerName << "\n";
        cout << "Balance: " << fixed << setprecision(2) << balance << "\n";
    }

    void showRecentTransactions(int n = 5) {
        cout << "Recent Transactions (up to " << n << "):\n";
        if (history.empty()) {
            cout << "  No transactions.\n";
            return;
        }
        int count = 0;
        for (int i = (int)history.size() - 1; i >= 0 && count < n; --i, ++count) {
            const Transaction &t = history[i];
            cout << "  " << t.type
                 << " | Amount: " << t.amount
                 << " | From: " << t.fromAcc
                 << " | To: " << t.toAcc
                 << " | Balance After: " << t.balanceAfter << "\n";
        }
    }
};

Customer* findCustomerById(vector<Customer> &customers, int id) {
    for (auto &c : customers) {
        if (c.id == id) return &c;
    }
    return nullptr;
}

Account* findAccountByNumber(vector<Account> &accounts, int accNo) {
    for (auto &a : accounts) {
        if (a.accountNumber == accNo) return &a;
    }
    return nullptr;
}

int main() {
    vector<Customer> customers;
    vector<Account> accounts;
    int nextCustomerId = 1;
    int nextAccountNumber = 1001;

    int choice;
    do {
        cout << "\n===== Simple Banking System =====\n";
        cout << "1. Create Customer\n";
        cout << "2. Create Account\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Transfer\n";
        cout << "6. View Account Info & Transactions\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            cout << "Enter customer name: ";
            cin.ignore();
            getline(cin, name);
            customers.emplace_back(nextCustomerId, name);
            cout << "Customer created. ID: " << nextCustomerId << "\n";
            nextCustomerId++;
        }
        else if (choice == 2) {
            int custId;
            double initial;
            cout << "Enter customer ID: ";
            cin >> custId;
            Customer *cust = findCustomerById(customers, custId);
            if (!cust) {
                cout << "Customer not found.\n";
            } else {
                cout << "Enter initial deposit: ";
                cin >> initial;
                if (initial < 0) initial = 0;
                accounts.emplace_back(nextAccountNumber, custId, initial);
                cout << "Account created. Account Number: " << nextAccountNumber << "\n";
                nextAccountNumber++;
            }
        }
        else if (choice == 3) {
            int accNo;
            double amount;
            cout << "Enter account number: ";
            cin >> accNo;
            Account *acc = findAccountByNumber(accounts, accNo);
            if (!acc) {
                cout << "Account not found.\n";
            } else {
                cout << "Enter amount to deposit: ";
                cin >> amount;
                acc->deposit(amount);
            }
        }
        else if (choice == 4) {
            int accNo;
            double amount;
            cout << "Enter account number: ";
            cin >> accNo;
            Account *acc = findAccountByNumber(accounts, accNo);
            if (!acc) {
                cout << "Account not found.\n";
            } else {
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                acc->withdraw(amount);
            }
        }
        else if (choice == 5) {
            int fromAccNo, toAccNo;
            double amount;
            cout << "Enter FROM account number: ";
            cin >> fromAccNo;
            cout << "Enter TO account number: ";
            cin >> toAccNo;
            Account *fromAcc = findAccountByNumber(accounts, fromAccNo);
            Account *toAcc   = findAccountByNumber(accounts, toAccNo);
            if (!fromAcc || !toAcc) {
                cout << "One or both accounts not found.\n";
            } else {
                cout << "Enter amount to transfer: ";
                cin >> amount;
                fromAcc->transferTo(*toAcc, amount);
            }
        }
        else if (choice == 6) {
            int accNo;
            cout << "Enter account number: ";
            cin >> accNo;
            Account *acc = findAccountByNumber(accounts, accNo);
            if (!acc) {
                cout << "Account not found.\n";
            } else {
                Customer *cust = findCustomerById(customers, acc->customerId);
                string name = cust ? cust->name : "(Unknown)";
                acc->showInfo(name);
                acc->showRecentTransactions(5);
            }
        }
        else if (choice == 0) {
            cout << "Exiting...\n";
        }
        else {
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}

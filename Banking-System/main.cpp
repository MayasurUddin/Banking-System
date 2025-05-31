#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class BankAccount {
private:
    string name;
    string username;
    string pin;
    int accountNumber;
    double balance;
    bool isActive;

public:
    static int nextAccountNumber;
    static constexpr double FDR_INTEREST = 0.06;
    static constexpr double DPS_INTEREST = 0.05;

    BankAccount() {
        name = "Unnamed";
        username = "";
        pin = "";
        accountNumber = ++nextAccountNumber;
        balance = 0;
        isActive = false;
    }

    void createAccount(string accName, string uname, string upin, double initialDeposit) {
        name = accName;
        username = uname;
        pin = upin;
        balance = initialDeposit;
        isActive = true;
        accountNumber = ++nextAccountNumber;
        cout << " Account created for " << name << " | Acc No: " << accountNumber << "\n";
    }

    bool login(string uname, string upin) {
        return (username == uname && pin == upin);
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Deposited $" << amount << ". New Balance: $" << balance << "\n";
    }

    void withdraw(double amount) {
        if (amount > balance) {
            cout << " Insufficient funds!\n";
            return;
        }
        balance -= amount;
        cout << "Withdrew $" << amount << ". New Balance: $" << balance << "\n";
    }

    void openFDR(double amount, int years) {
        if (amount > balance) {
            cout << " Not enough balance for FDR.\n";
            return;
        }
        double total = amount * (1 + FDR_INTEREST * years);
        balance -= amount;
        cout << " FDR of $" << amount << " opened for " << years << " years.\n";
        cout << " Returns after " << years << " years: $" << total << "\n";
    }

    void openDPS(double monthlyAmount, int months) {
        double total = monthlyAmount * months * (1 + DPS_INTEREST);
        cout << " DPS: $" << monthlyAmount << "/month for " << months << " months.\n";
        cout << " Maturity Return: $" << total << "\n";
    }

    void showInfo() {
        cout << " Name: " << name << ", Acc No: " << accountNumber << ", Balance: $" << balance << "\n";
    }

    string getUsername() { return username; }
    bool isAccountActive() { return isActive; }
    string getPIN() { return pin; }
    int getAccountNumber() { return accountNumber; }
    double getBalance() { return balance; }
    string getName() { return name; }

    void load(string uname, string upin, string accName, int accNum, double bal) {
        username = uname;
        pin = upin;
        name = accName;
        accountNumber = accNum;
        balance = bal;
        isActive = true;
        if (accNum > nextAccountNumber)
            nextAccountNumber = accNum;
    }
};

int BankAccount::nextAccountNumber = 1000;

const int maxUsers = 10;
BankAccount users[maxUsers];
int userCount = 0;

void saveToFile() {
    ofstream file("accounts.txt");
    for (int i = 0; i < userCount; i++) {
        if (users[i].isAccountActive()) {
            file << users[i].getUsername() << " "
                 << users[i].getPIN() << " "
                 << users[i].getName() << " "
                 << users[i].getAccountNumber() << " "
                 << users[i].getBalance() << "\n";
        }
    }
    file.close();
}

void loadFromFile() {
    ifstream file("accounts.txt");
    if (!file.is_open()) return;

    string uname, pin, name;
    int accNum;
    double balance;

    while (file >> uname >> pin >> name >> accNum >> balance) {
        users[userCount].load(uname, pin, name, accNum, balance);
        userCount++;
    }
    file.close();
}

void showMenu(BankAccount &account) {
    int choice;
    do {
        cout << "\n===== BANK MENU =====\n";
        cout << "1. Deposit\n2. Withdraw\n3. Open FDR\n4. Open DPS\n5. Show Account Info\n6. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                double amt;
                cout << "Enter deposit amount: $";
                cin >> amt;
                account.deposit(amt);
                break;
            }
            case 2: {
                double amt;
                cout << "Enter withdrawal amount: $";
                cin >> amt;
                account.withdraw(amt);
                break;
            }
            case 3: {
                double amt;
                int years;
                cout << "Enter amount for FDR: $";
                cin >> amt;
                cout << "Enter duration (years): ";
                cin >> years;
                account.openFDR(amt, years);
                break;
            }
            case 4: {
                double monthly;
                int months;
                cout << "Enter monthly amount for DPS: $";
                cin >> monthly;
                cout << "Enter duration (months): ";
                cin >> months;
                account.openDPS(monthly, months);
                break;
            }
            case 5:
                account.showInfo();
                break;
            case 6:
                cout << "Logging out...\n";
                break;
            default:
                cout << " Invalid choice. Try again.\n";
        }
        saveToFile(); // Save changes after every transaction
    } while (choice != 6);
}

int main() {
    loadFromFile();

    int option;
    while (true) {
        cout << "\n=== Welcome to File-Based Bank System ===\n";
        cout << "1. Create Account\n2. Login\n3. Exit\n";
        cout << "Choose an option: ";
        cin >> option;

        if (option == 1) {
            if (userCount >= maxUsers) {
                cout << "Max account limit reached!\n";
                continue;
            }
            string name, uname, pin;
            double initial;
            cout << "Enter Name: ";
            cin >> name;
            cout << "Choose Username: ";
            cin >> uname;
            cout << "Set 4-digit PIN: ";
            cin >> pin;
            cout << "Initial Deposit: $";
            cin >> initial;

            users[userCount].createAccount(name, uname, pin, initial);
            userCount++;
            saveToFile();
        }
        else if (option == 2) {
            string uname, pin;
            cout << "Username: ";
            cin >> uname;
            cout << "PIN: ";
            cin >> pin;

            bool found = false;
            for (int i = 0; i < userCount; i++) {
                if (users[i].login(uname, pin)) {
                    cout << "Login successful. Welcome " << uname << "!\n";
                    showMenu(users[i]);
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Invalid credentials.\n";
            }
        }
        else if (option == 3) {
            cout << "Saving data and exiting...\n";
            saveToFile();
            break;
        }
        else {
            cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}


#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <unistd.h> // for sleep() in UNIX-based systems

using namespace std;

const int MAX_CUSTOMERS = 100;

struct Transaction {
    string type;
    double amount;
};

struct Account {
    string username;
    string password;
    double balance = 0;
    Transaction transactions[100];
    int transactionCount = 0;
};

Account accounts[MAX_CUSTOMERS];
int accountCount = 0;
int currentIndex = -1;

void saveAccountsToFile() {
    ofstream fout("accounts.txt");
    fout << accountCount << endl;
    for (int i = 0; i < accountCount; i++) {
        fout << accounts[i].username << " " << accounts[i].password << " "
             << accounts[i].balance << " " << accounts[i].transactionCount << endl;
        for (int j = 0; j < accounts[i].transactionCount; j++) {
            fout << accounts[i].transactions[j].type << " " << accounts[i].transactions[j].amount << endl;
        }
    }
    fout.close();
}

void loadAccountsFromFile() {
    ifstream fin("accounts.txt");
    if (!fin) return;

    fin >> accountCount;
    for (int i = 0; i < accountCount; i++) {
        fin >> accounts[i].username >> accounts[i].password >> accounts[i].balance >> accounts[i].transactionCount;
        for (int j = 0; j < accounts[i].transactionCount; j++) {
            fin >> accounts[i].transactions[j].type >> accounts[i].transactions[j].amount;
        }
    }
    fin.close();
}

bool validateAmount(double amount) {
    return amount > 0;
}

bool usernameExists(string username) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].username == username) return true;
    }
    return false;
}

void registerAccount() {
    if (accountCount >= MAX_CUSTOMERS) {
        cout << "Maximum customer limit reached!" << endl;
        return;
    }

    string uname, pass;
    cout << "Enter new username: ";
    cin >> uname;

    if (usernameExists(uname)) {
        cout << "Username already exists. Try another." << endl;
        return;
    }

    cout << "Enter password: ";
    cin >> pass;

    accounts[accountCount].username = uname;
    accounts[accountCount].password = pass;
    accounts[accountCount].balance = 0;
    accounts[accountCount].transactionCount = 0;
    accountCount++;
    saveAccountsToFile();

    cout << "Account registered successfully!\n";
}

bool login() {
    string uname, pass;
    cout << "Username: ";
    cin >> uname;
    cout << "Password: ";
    cin >> pass;

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].username == uname && accounts[i].password == pass) {
            currentIndex = i;
            return true;
        }
    }
    return false;
}

void deposit() {
    double amt;
    cout << "Enter amount to deposit: ";
    cin >> amt;
    if (!validateAmount(amt)) {
        cout << "Invalid amount!" << endl;
        return;
    }

    accounts[currentIndex].balance += amt;
    accounts[currentIndex].transactions[accounts[currentIndex].transactionCount++] = {"Deposit", amt};
    saveAccountsToFile();
    cout << "Deposit successful!" << endl;
}

void withdraw() {
    double amt;
    cout << "Enter amount to withdraw: ";
    cin >> amt;
    if (!validateAmount(amt) || amt > accounts[currentIndex].balance) {
        cout << "Invalid or insufficient amount!" << endl;
        return;
    }

    accounts[currentIndex].balance -= amt;
    accounts[currentIndex].transactions[accounts[currentIndex].transactionCount++] = {"Withdraw", amt};
    saveAccountsToFile();
    cout << "Withdrawal successful!" << endl;
}

void transfer() {
    string receiver;
    double amt;
    cout << "Enter recipient username: ";
    cin >> receiver;

    int receiverIndex = -1;
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].username == receiver) {
            receiverIndex = i;
            break;
        }
    }

    if (receiverIndex == -1 || receiverIndex == currentIndex) {
        cout << "Invalid recipient!" << endl;
        return;
    }

    cout << "Enter amount to transfer: ";
    cin >> amt;

    if (!validateAmount(amt) || amt > accounts[currentIndex].balance) {
        cout << "Invalid or insufficient amount!" << endl;
        return;
    }

    accounts[currentIndex].balance -= amt;
    accounts[receiverIndex].balance += amt;

    accounts[currentIndex].transactions[accounts[currentIndex].transactionCount++] = {"TransferTo", amt};
    accounts[receiverIndex].transactions[accounts[receiverIndex].transactionCount++] = {"TransferFrom", amt};

    saveAccountsToFile();
    cout << "Transfer successful!" << endl;
}

void viewBalance() {
    cout << "Current Balance: Rs. " << fixed << setprecision(2) << accounts[currentIndex].balance << endl;
}

void viewTransactions() {
    cout << "Recent Transactions:\n";
    for (int i = 0; i < accounts[currentIndex].transactionCount; i++) {
        cout << i + 1 << ". " << accounts[currentIndex].transactions[i].type
             << " Rs. " << accounts[currentIndex].transactions[i].amount << endl;
    }
}

void animateWelcome() {
    string banner = "=== Welcome to C++ Bank ===";
    for (char ch : banner) {
        cout << ch;
        usleep(40000); // 40ms delay
    }
    cout << "\n---------------------------\n";
}

void mainMenu() {
    int choice;
    do {
        cout << "\n1. Deposit\n2. Withdraw\n3. Transfer\n4. View Balance\n5. View Transactions\n6. Logout\nChoice: ";
        cin >> choice;

        switch (choice) {
            case 1: deposit(); break;
            case 2: withdraw(); break;
            case 3: transfer(); break;
            case 4: viewBalance(); break;
            case 5: viewTransactions(); break;
            case 6: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 6);
}

int main() {
    loadAccountsFromFile();
    animateWelcome();

    while (true) {
        int opt;
        cout << "\n1. Login\n2. Register\n3. Exit\nChoice: ";
        cin >> opt;

        switch (opt) {
            case 1:
                if (login()) {
                    cout << "Login successful!\n";
                    mainMenu();
                } else {
                    cout << "Invalid credentials!\n";
                }
                break;
            case 2: registerAccount(); break;
            case 3:
                cout << "Thank you for banking with us!" << endl;
                return 0;
            default:
                cout << "Invalid option!\n";
        }
    }

    return 0;
}

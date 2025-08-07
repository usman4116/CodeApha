#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

// Struct for user data
struct User {
    string username;
    string password;
};

// Function to clear the screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Print with character delay (typing effect)
void printWithDelay(const string& text, int delayMs = 15) {
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}

// Simple loading animation
void loadingAnimation(string msg = "Loading", int dotCount = 3, int cycles = 2) {
    for (int j = 0; j < cycles; j++) {
        cout << msg;
        for (int i = 0; i < dotCount; i++) {
            cout << "." << flush;
            this_thread::sleep_for(chrono::milliseconds(300));
        }
        cout << "\r" << string(msg.size() + dotCount, ' ') << "\r";
    }
}

// Function to pause the screen
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Function to print the main menu
void printMenu() {
    clearScreen();
    printWithDelay("=============================\n");
    printWithDelay("         Login System\n");
    printWithDelay("=============================\n");
    printWithDelay("1. Register\n");
    printWithDelay("2. Login\n");
    printWithDelay("3. Change Password\n");
    printWithDelay("4. Delete Account\n");
    printWithDelay("5. View All Users (Admin)\n");
    printWithDelay("6. Exit\n");
    printWithDelay("=============================\n");
    cout << "Enter your choice: ";
}

// Load users from file
vector<User> loadUsers() {
    vector<User> users;
    ifstream inFile("users.txt");
    if (inFile.is_open()) {
        User user;
        while (inFile >> user.username >> user.password) {
            users.push_back(user);
        }
        inFile.close();
    }
    return users;
}

// Save users to file
void saveUsers(const vector<User>& users) {
    ofstream outFile("users.txt", ios::trunc);
    for (const auto& user : users) {
        outFile << user.username << " " << user.password << endl;
    }
    outFile.close();
}

// Register new user
void registerUser() {
    clearScreen();
    string username, password;
    printWithDelay("==== User Registration ====\n");
    cout << "Enter a username: ";
    cin >> username;
    cout << "Enter a password: ";
    cin >> password;

    vector<User> users = loadUsers();
    users.push_back({username, password});
    saveUsers(users);

    printWithDelay("Registering", 100);
    loadingAnimation();
    printWithDelay("\nRegistration successful!\n");
    pauseScreen();
}

// Login user
bool loginUser(string& loggedInUser) {
    clearScreen();
    string username, password;
    printWithDelay("======= User Login =======\n");
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    vector<User> users = loadUsers();
    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            loggedInUser = username;
            printWithDelay("Logging in", 100);
            loadingAnimation();
            return true;
        }
    }
    return false;
}

// Change password
void changePassword(const string& loggedInUser) {
    clearScreen();
    string oldPassword, newPassword;
    printWithDelay("==== Change Password ====\n");
    cout << "Enter old password: ";
    cin >> oldPassword;
    cout << "Enter new password: ";
    cin >> newPassword;

    vector<User> users = loadUsers();
    for (auto& user : users) {
        if (user.username == loggedInUser && user.password == oldPassword) {
            user.password = newPassword;
            saveUsers(users);
            printWithDelay("Updating password", 100);
            loadingAnimation();
            printWithDelay("\nPassword changed successfully!\n");
            pauseScreen();
            return;
        }
    }

    printWithDelay("Incorrect old password!\n");
    pauseScreen();
}

// Delete user
void deleteUser(const string& loggedInUser) {
    clearScreen();
    string password;
    printWithDelay("==== Delete Account ====\n");
    cout << "Enter your password to confirm: ";
    cin >> password;

    vector<User> users = loadUsers();
    auto it = remove_if(users.begin(), users.end(), [&](const User& user) {
        return user.username == loggedInUser && user.password == password;
    });

    if (it != users.end()) {
        users.erase(it, users.end());
        saveUsers(users);
        printWithDelay("Deleting account", 100);
        loadingAnimation();
        printWithDelay("\nAccount deleted successfully!\n");
        pauseScreen();
        exit(0);
    } else {
        printWithDelay("Incorrect password!\n");
    }

    pauseScreen();
}

// View all users (admin)
void viewAllUsers() {
    clearScreen();
    string adminPassword;
    printWithDelay("==== Admin Panel ====\n");
    cout << "Enter admin password: ";
    cin >> adminPassword;

    if (adminPassword == "admin") {
        vector<User> users = loadUsers();
        printWithDelay("Fetching users", 100);
        loadingAnimation();
        cout << "\nRegistered Users:\n";
        for (const auto& user : users) {
            cout << " - " << user.username << "\n";
        }
    } else {
        printWithDelay("Access Denied! Incorrect password.\n");
    }

    pauseScreen();
}

// Main Function
int main() {
    int choice;
    string loggedInUser;
    bool isRunning = true;

    clearScreen();
    printWithDelay("Welcome to the Secure Login System!\n", 25);
    loadingAnimation("Initializing");
    while (isRunning) {
        printMenu();
        cin >> choice;

        while (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter a number: ";
            cin >> choice;
        }

        switch (choice) {
            case 1: registerUser(); break;
            case 2:
                if (loginUser(loggedInUser)) {
                    printWithDelay("\nLogin successful!\n");
                } else {
                    printWithDelay("\nInvalid credentials!\n");
                }
                pauseScreen();
                break;
            case 3:
                if (!loggedInUser.empty()) changePassword(loggedInUser);
                else {
                    printWithDelay("Please login first.\n");
                    pauseScreen();
                }
                break;
            case 4:
                if (!loggedInUser.empty()) deleteUser(loggedInUser);
                else {
                    printWithDelay("Please login first.\n");
                    pauseScreen();
                }
                break;
            case 5: viewAllUsers(); break;
            case 6:
                char confirm;
                cout << "Are you sure you want to exit? (y/n): ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') isRunning = false;
                break;
            default:
                printWithDelay("Invalid choice. Try again.\n");
                pauseScreen();
        }
    }

    printWithDelay("Exiting", 100);
    loadingAnimation();
    printWithDelay("\nGoodbye!\n");

    return 0;
}

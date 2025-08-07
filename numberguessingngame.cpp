#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h> // For Sleep and system("cls")
using namespace std;

void showLoadingBar(string text = "Loading") {
    cout << "\n" << text;
    for (int i = 0; i < 10; i++) {
        cout << ".";
        Sleep(150);
    }
    cout << "\n";
}

void printHeader() {
    system("cls");
    cout << "=========================================\n";
    cout << "         WELCOME TO GUESSING GAME        \n";
    cout << "=========================================\n";
    showLoadingBar("Initializing Game");
    system("cls");
}

void printWithBorder(string text) {
    cout << "\n+---------------------------------------+\n";
    cout << "| " << text << "\n";
    cout << "+---------------------------------------+\n";
}

int main() {
    srand((unsigned)time(0)); // Seed only once

    char op;
    do {
        printHeader();

        int random = (rand() % 100) + 1;
        int guess;
        int turns_left = 5;
        bool guessedCorrectly = false;

        printWithBorder("Guess a number between 1 and 100");
        cout << "\nYou have 5 tries to guess it right!\n";

        while (turns_left > 0) {
            cout << "\nTurns left: " << turns_left << endl;
            cout << "Enter your guess: ";
            cin >> guess;

            if (guess > random) {
                cout << "Too high! Try a smaller number." << endl;
            } else if (guess < random) {
                cout << "Too low! Try a larger number." << endl;
            } else {
                cout << "\nYou guessed the number!\n";
                cout << "Congratulations, you won the game!\n";
                guessedCorrectly = true;
                break;
            }
            turns_left--;
        }

        if (!guessedCorrectly) {
            cout << "\nOh no! Your tries have finished.\n";
            cout << "The secret number was: " << random << endl;
        }

        showLoadingBar("Preparing Results");

        cout << "\n-----------------------------------------\n";
        cout << "              GAME OVER                  \n";
        cout << "-----------------------------------------\n";

        cout << "Would you like to play again? (y/n): ";
        cin >> op;

        if (op == 'y' || op == 'Y') {
            showLoadingBar("Restarting Game");
        }

    } while (op == 'y' || op == 'Y');

    showLoadingBar("Exiting");
    cout << "\nThanks for playing. Goodbye!\n";

    return 0;
}

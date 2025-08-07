#include <iostream>
#include <unistd.h>   // for usleep
using namespace std;

const int SIZE = 9;

// ANSI color codes
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string CYAN = "\033[36m";
const string YELLOW = "\033[33m";
const string BOLD = "\033[1m";

// Function to clear screen (cross-platform basic)
void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

// Function to print the Sudoku board with formatting
void printBoard(int board[SIZE][SIZE]) {
    clearScreen();
    cout << BOLD << CYAN << "======= SUDOKU SOLVER =======\n" << RESET;
    for (int row = 0; row < SIZE; ++row) {
        if (row % 3 == 0)
            cout << CYAN << "-------------------------------\n" << RESET;

        for (int col = 0; col < SIZE; ++col) {
            if (col % 3 == 0) cout << CYAN << "| " << RESET;

            if (board[row][col] == 0)
                cout << RED << ". " << RESET;
            else
                cout << GREEN << board[row][col] << " " << RESET;
        }
        cout << CYAN << "|" << RESET << endl;
    }
    cout << CYAN << "-------------------------------" << RESET << endl;
}

// Validate placement
bool isValid(int board[SIZE][SIZE], int row, int col, int num) {
    for (int x = 0; x < SIZE; ++x)
        if (board[row][x] == num || board[x][col] == num)
            return false;

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i + startRow][j + startCol] == num)
                return false;

    return true;
}

// Find empty cell
bool findEmpty(int board[SIZE][SIZE], int &row, int &col) {
    for (row = 0; row < SIZE; ++row)
        for (col = 0; col < SIZE; ++col)
            if (board[row][col] == 0)
                return true;
    return false;
}

// Solve Sudoku with animation
bool solveSudoku(int board[SIZE][SIZE]) {
    int row, col;
    if (!findEmpty(board, row, col))
        return true;

    for (int num = 1; num <= 9; ++num) {
        if (isValid(board, row, col, num)) {
            board[row][col] = num;
            printBoard(board);
            usleep(50000); // 50ms delay

            if (solveSudoku(board))
                return true;

            board[row][col] = 0; // backtrack
        }
    }
    return false;
}

// Main function
int main() {
    int board[SIZE][SIZE] = {
        {5, 3, 0, 0, 7, 0, 0, 1, 2},
        {6, 0, 0, 1, 9, 5, 0, 0, 8},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 5, 0, 0, 6, 0, 0, 0, 3},
        {4, 2, 0, 8, 0, 3, 0, 0, 1},
        {7, 1, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 4, 5, 0, 8, 0, 0, 7, 9}
    };

    printBoard(board);
    cout << YELLOW << "\nSolving..." << RESET << endl;
    sleep(1);

    if (solveSudoku(board)) {
        printBoard(board);
        cout << GREEN << "\nSudoku Solved Successfully!" << RESET << endl;
    } else {
        cout << RED << "\nNo solution exists!" << RESET << endl;
    }

    return 0;
}

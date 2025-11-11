/*
tic-tac-toe in c
PvP and PvE (minimax)
compile: gcc -o main main.c
run: ./main

control:
enter [row] [column]
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SIZE 3
#define SAVE_FILE "save.txt"

// Board 'X', 'O' OR '_'
char board[SIZE][SIZE];

// clear screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// initialize board
void initBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = '_';
        }
    }
}

// display board
void printBoard() {
    clearScreen();
    printf("\n    1   2   3\n");
    printf("  -------------\n");
    for (int i = 0; i < SIZE; ++i) {
        printf("%d |", i + 1);
        for (int j = 0; j < SIZE; ++j) {
            printf(" %c ", board[i][j]);
            if (j < SIZE - 1) printf("|");
        }
        printf("|\n");
        if (i < SIZE - 1) printf("  -------------\n");
    }
    printf("  -------------\n\n");
}

int movesLeft() {
    for (int i = 0; i < SIZE; i++) {
        for (int  j = 0; j < SIZE; j++) {
            if (board[i][j] == '_') return 1;
        }
    }
    return 0;
}

// evalute score
int evaluate() {
    for (int row = 0; row < SIZE; ++row) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == 'O') return +10;
            else if (board[row][0] == 'X') return -10;
        }
    }
    for (int col = 0; col < SIZE; ++col) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == 'O') return +10;
            else if (board[0][col] == 'X') return -10;
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == 'O') return +10;
        else if (board[0][0] == 'X') return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == 'O') return +10;
        else if (board[0][2] == 'X') return -10;
    }
    return 0;
}

int minimax(int depth, int isMax) {
    int score = evaluate();
    if (score == 10) return score - depth;
    if (score == -10) return score + depth;
    if (!movesLeft()) return 0;

    if (isMax) {
        int best = INT_MIN;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == '_') {
                    board[i][j] = 'O';
                    int val = minimax(depth + 1, 0);
                    board[i][j] = '_';
                    if (val > best) best = val;
                }
            }
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == '_') {
                    board[i][j] = 'X';
                    int val = minimax(depth + 1, 1);
                    board[i][j] = '_';
                    if (val < best) best = val;
                }
            }
        }
        return best;
    }
}

// find best move for ai
void findBestMove(int *bestRow, int *bestCol) {
    int bestVal = INT_MIN;
    *bestRow = -1;
    *bestCol = -1;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == '_') {
                board[i][j] = 'O';
                int moveVal = minimax(0, 0);
                board[i][j] = '_';
                if (moveVal > bestVal) {
                    *bestRow = i;
                    *bestCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }
}

int checkWin() {
    int score = evaluate();
    if (score == 10) return 1;
    if (score == -10) return -1;
    if (!movesLeft()) return 0;
    return 2;
}

void saveGame(int mode, int turn) {
    FILE *f = fopen(SAVE_FILE, "w");
    if (!f) return;
    fprintf(f, "%d %d\n", mode, turn);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(f, "%c", board[i][j]);
        }
        fprintf(f, "\n");
    } 
    fclose(f);
}

int loadGame(int *mode, int *turn) {
    FILE *f = fopen(SAVE_FILE, "r");
    if (!f) return 0;

    if (fscanf(f, "%d %d", mode, turn) != 2) {
        fclose(f);
        return 0;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (fscanf(f, " %c", &board[i][j]) != 1) {
                fclose(f);
                return 0;
			}
        }
    }
	fclose(f);
	return 1;
}

void playerMove(char player) {
    int r, c;
    while (1) {
        printf("player %c, enter [row] and [column] (1-3) or 0 0 to save & exit: ", player);
        if (scanf("%d %d", &r, &c) != 2) {
            while (getchar() != '\n');
            printf("invalid input. use two numbers between 1 and 3.\n");
            continue;
        }
        if (r == 0 && c == 0) {
            printf("game saved. exited\n");
            exit(0);
        }
        if (r < 1 || r > 3 || c < 1 || c > 3) {
            printf("row and column must be 1..3.\n");
            continue;
        }
        if (board[r - 1][c - 1] != '_') {
            printf("cell already taken. pick another.\n");
            continue;
        }
        board[r - 1][c - 1] = player;
        break;
    }
}

void aiMove() {
    int r, c;
    findBestMove(&r, &c);
    if (r == -1 || c == -1) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == '_') {
                    r = i;
                    c = j;
                    goto placed;
                }
            }
        }
    }
    placed:
        board[r][c] = 'O';
        printf("ai placed O at %d %d\n", r + 1, c + 1);
}

int main() {
    int mode = 0, turn = 0;
    if (loadGame(&mode, &turn)) {
        char choice;
        printf("saved game found. load it? (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            printf("resuming saved game.\n");
        } else {
            initBoard();
            printf("starting new game\n");
        }
    } else {
        initBoard();
        printf("tic-tac-toe\n");
        printf("1.PvP\n2.PvE (you play X)\nchoose mode (1 or 2): ");
        while (scanf("%d", &mode) != 1 || (mode != 1 && mode != 2)) {
            while (getchar() != '\n') {
                printf("invalid input. choose 1 or 2: ");
            }
        }
    }

    while (1) {
        printBoard();
        int state = checkWin();
        if (state == 1) {
            printf("O wins!\n");
            remove(SAVE_FILE);
            break;
        }
        if (state == -1) {
            printf("X wins!\n");
            remove(SAVE_FILE);
            break;
        }
        if (state == 0) {
            printf("It's a draw!\n");
            remove(SAVE_FILE);
            break;
        }

        if (mode == 1) {
            if (turn == 0) playerMove('X');
            else playerMove('O');
        } else {
            if (turn == 0) playerMove('X');
            else aiMove();
        }

        turn = 1 - turn;
        saveGame(mode, turn);
        printf("(Auto-saved progress)\n");
    }

    return 0;
}

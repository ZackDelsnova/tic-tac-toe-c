/*
tic-tac-toe in c
PvP and PvE
compile: gcc -o main main.c
run: ./main

control:
enter [row] [column]
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SIZE 3

// Board 'X', 'O' OR '_'
char board[SIZE][SIZE];

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
    puts("\n  1 2 3");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i+1);
        for (int j = 0; j < SIZE; j++) {
            printf("%c", board[i][j]);
            if (j < SIZE - 1) putchar(' ');
        }
        putchar('\n');
    }
    putchar('\n');
}

int main() {
    initBoard();
    printBoard();
    return 0;
}

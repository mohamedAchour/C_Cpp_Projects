#ifndef _GAME_H
#define _GAME_H

#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
// XMAX y YMAX must be multiple of 4.
#define XMAX 32
#define YMAX 16

#define WIDTH  8
#define HEIGHT 4

#define BOARD_SIZE 4

typedef struct {
    int x;  //table origin : x
    int y;  //table origin : y
    char s_board[BOARD_SIZE][BOARD_SIZE]; 
    int cursorig[2]; //Cursor origines
    int cursdest[2]; //Cursor destination
    char* foundWord; //Selectd word by the user
    int wordLength;  //The length of the selected word
    int score;       //Game score
    int nbr_guessedWords; //The number of selected words
}board;

void initializeTabel(board *Tboard);
void fill_grid(board *Tboard, int n);
void drawboard(board *Tboard);
void placecursor(board *Tboard);
void selectLetter(board *Tboard);
void computeScore(board *Tboard);
bool checkWord(board *Tboard);
int maingame(board  *Tboard);


#endif
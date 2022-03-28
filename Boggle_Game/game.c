#include"game.h"

//Initialize game data
void initializeTabel(board *Tboard){
    Tboard->cursorig[0] = 0;
    Tboard->cursorig[1] = 0;
    Tboard->cursdest[0] = 0;
    Tboard->cursdest[1] = 0;

    Tboard->wordLength = 0;
    Tboard->foundWord = (char *)malloc(sizeof(char)); 
    Tboard->score = 0;
    Tboard->nbr_guessedWords = 0;
}

//Fill the table with randomly generated letters
void fill_grid(board  *Tboard, int n){
    int i, j;
    char k;
    const int number_of_chars = 100;

    //Table of letters is generated according to the following distribution: 
    // E :             11%
    // T :             8%
    // {A,I,N,O,S} :   7%
    // R :             6%
    // H:              5%
    // {D,L}:          4% 
    // {C,M,U} :       3%
    // {B,F,G,P,W,Y} : 2%
    // {J,K,Q,V,X,Z} : 1%
    
    const char block_distribution[100] = {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E',\
                                      'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T',\
                                      'A', 'I', 'N', 'O', 'S', 'A', 'I', 'N', 'O', 'S', 'A', 'I', 'N', 'O', 'S', 'A', 'I', 'N', 'O', 'S',\
                                      'A', 'I', 'N', 'O', 'S','A', 'I', 'N', 'O', 'S','A', 'I', 'N', 'O', 'S',\
                                      'R', 'R', 'R', 'R', 'R', 'R',\
                                      'H', 'H', 'H', 'H', 'H',\
                                      'D', 'L', 'D', 'L', 'D', 'L', 'D', 'L',\
                                      'C' ,'M', 'U', 'C', 'M', 'U', 'C' ,'M', 'U',\
                                      'B', 'F', 'G', 'P', 'W', 'Y', 'B', 'F', 'G', 'P', 'W' ,'Y',\
                                      'J', 'K', 'Q', 'V', 'X', 'Z'};
	srand(time(NULL));
	for (i = 0;i < n;++i)
		for(j = 0;j < n; ++j,++k)
		{
            //pick a number between 0 and 99
            k = (rand() % number_of_chars);
			Tboard->s_board[i][j] = block_distribution[k];
		}
}

void drawboard(board  *Tboard)		// Draw gameboard
{
    int i, j;

    for(i=0; i<=YMAX; i++)
        for(j=0; j<=XMAX; j++)
        {
            if (i == 0)   //First row of the table
            {
                if (j == 0)              //UP-LEFT corner.
                    mvaddch(Tboard->y+i, Tboard->x+j, ACS_ULCORNER);
                else if (j == XMAX)     //UP-RIGHT corner.
                    mvaddch(Tboard->y+i, Tboard->x+j, ACS_URCORNER);
                else if (j % (XMAX / 4) == 0)   //Midlle column
                    mvaddch(Tboard->y+i, Tboard->x+j, ACS_TTEE);
                else                    //Normal lines.
                    mvaddch(Tboard->y+i, Tboard->x+j, ACS_HLINE);
            }

            else if (i % (YMAX / 4) == 0 && i != YMAX)   //Dividers.
            {
                if (j == 0)             //First column.
                    mvaddch(Tboard->y+i, Tboard->x+j, ACS_LTEE);
                else if (j == XMAX)     //Last coloumn.
                    mvaddch(Tboard->y+i, Tboard->x+j, ACS_RTEE);
                else if (j % (XMAX / 4) == 0)   //Midlle column.
                    mvaddch(Tboard->y+i, Tboard->x+j, ACS_PLUS);
                else                    //Normal lines.
                    mvaddch(Tboard->y+i, Tboard->x+j, ACS_HLINE);
            }

            else if (i == YMAX)            //Last row.
            {
                if (j == 0)              //Lower-left corner. 
                    mvaddch(Tboard->y+i, Tboard->x+j, ACS_LLCORNER);
                else if (j == XMAX)     //Lower-right corner. 
                    mvaddch(Tboard->y+i, Tboard->x+j, ACS_LRCORNER);
                else if (j % (XMAX / 4) == 0)   //Midlle column.
                    mvaddch(Tboard->y+i, Tboard->x+j, ACS_BTEE);
                else                    //Normal lines.
                    mvaddch(Tboard->y+i, Tboard->x+j, ACS_HLINE);
            }
            
            else if (j % (XMAX / 4) == 0)   //Midlle lines.
            {
                mvaddch(Tboard->y+i, Tboard->x+j, ACS_VLINE);
            }

        }

    //Add letters to the table.
    int deltay = HEIGHT / 2;
	int deltax = WIDTH  / 2;
    for(j = 0; j < BOARD_SIZE; ++j)
        for(i = 0;i < BOARD_SIZE; ++i)
        {
                attron(COLOR_PAIR(1));
                mvprintw(Tboard->y + j * HEIGHT + deltay, Tboard->x + i * WIDTH  + deltax, "%-2c", Tboard->s_board[i][j]);
                attroff(COLOR_PAIR(1));
        }
}

void placecursor(board  *Tboard)
{
    int i, j;
    int y, x;
    
    for (i=0; i<3; i++)
    {
        //Clean the last cursor
        y = YMAX/4 * Tboard->cursorig[0] + i + Tboard->y + 1;
        x = XMAX/4 * Tboard->cursorig[1] + Tboard->x + 1;
        mvaddch(y, x, ' ');
        x = x + 6;
        mvaddch(y, x, ' ');
        
        //Define the new cursor. 
        y = YMAX/4 * Tboard->cursdest[0] + i + Tboard->y + 1;
        x = XMAX/4 * Tboard->cursdest[1] + Tboard->x + 1;
        
        attron(COLOR_PAIR(3) | A_BLINK);
        mvaddch(y, x, ACS_DIAMOND);
        x = x + 6;
        mvaddch(y, x, ACS_DIAMOND);
        attroff(COLOR_PAIR(3) | A_BLINK);
    }
}

void selectLetter(board  *Tboard){
    int deltay = HEIGHT / 2;
	int deltax = WIDTH  / 2;

    //The chosen letter from the table is determined using the current position of the cursor
    int selected_letter_x = Tboard->cursorig[0];
    int selected_letter_y = Tboard->cursorig[1];

    Tboard->foundWord = (char *)realloc(Tboard->foundWord, (Tboard->wordLength+1)*(sizeof(char)));
    Tboard->foundWord[Tboard->wordLength] = tolower(Tboard->s_board[selected_letter_y][selected_letter_x]);
    ++Tboard->wordLength;

    attron(COLOR_PAIR(2));
    mvprintw(Tboard->y + selected_letter_x*HEIGHT + deltay, Tboard->x + selected_letter_y*WIDTH  + deltax, "%-2c", Tboard->s_board[selected_letter_y][selected_letter_x]);
    attroff(COLOR_PAIR(1));
}

void computeScore(board *Tboard){
    int sum = 0;
    //The score is calculated as follow
    if(Tboard->wordLength >= 3){
        sum = pow(2, Tboard->wordLength);   
        Tboard->score = Tboard->score  + sum;
    }
}

bool checkWord(board *Tboard){
	FILE* filePointer;
	bool wordExist = false;
	int bufferLength = 255;
	char line[bufferLength];

    //TODO: Optimize the search of words(Binary search algorithm)
	filePointer = fopen("words.txt", "r");
	while(fgets(line, bufferLength, filePointer))
	{
		if (strcmp(line, Tboard->foundWord) == 0) 
		{
			wordExist = true;
			break;
		}
	}
	fclose(filePointer);
	if (wordExist)
	{
        computeScore(Tboard);
		return true;
	}
	else 
	{
		return false;
	}
}

int maingame(board *Tboard)
{
    int key = 0;
    int gameEnded  = 0;

    initializeTabel(Tboard);
    placecursor(Tboard);
    
    while (key != '0' && gameEnded == 0)
    {
        mvprintw(5, 80, "CURRENT SCORE:");
        key = getch();
        switch (key)
        {
            case KEY_LEFT:
                if (Tboard->cursorig[1] > 0)
                {
                    Tboard->cursdest[1] = Tboard->cursorig[1] - 1;
                    placecursor(Tboard);
                    Tboard->cursorig[1] = Tboard->cursdest[1];
                }
                break;
            case KEY_RIGHT:
                if (Tboard->cursorig[1] < 3)
                {
                    Tboard->cursdest[1] = Tboard->cursorig[1] + 1;
                    placecursor(Tboard);
                    Tboard->cursorig[1] = Tboard->cursdest[1];
                }
                break;

            case KEY_UP:
                if (Tboard->cursorig[0] > 0)
                {
                    Tboard->cursdest[0] = Tboard->cursorig[0] - 1;
                    placecursor(Tboard);
                    Tboard->cursorig[0] = Tboard->cursdest[0];
                }
                break;

            case KEY_DOWN:
                if (Tboard->cursorig[0] < 3)
                {
                    Tboard->cursdest[0] = Tboard->cursorig[0] + 1;
                    placecursor(Tboard);
                    Tboard->cursorig[0] = Tboard->cursdest[0];
                }
                break;
            
            case ' ':
                selectLetter(Tboard);
                break;
            
            case 10: 
                //Count the number of selected words: 4 attempts max.
                Tboard->nbr_guessedWords++;
                Tboard->foundWord[Tboard->wordLength] = '\n'; 
                bool word_exist = checkWord(Tboard);
                mvprintw(7, 86, "%d", Tboard->score);
                if(word_exist){
                    mvprintw(21, (80-10)/2, "Word exist");
                }
                else{
                    mvprintw(21, (80-21)/2, "Word does not exist");
                }
                Tboard->wordLength = 0;
                if(Tboard->nbr_guessedWords == 4){
                    gameEnded = 1;
                }
                break;
        }
    }
    return gameEnded;
}

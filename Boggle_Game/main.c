#include"game.h"

int main()
{
    int key;		// pressed key
    int exit = 0;	// Exit
    board  Tboard;
    int gameEnded = 0;

    // Center the table on the screen
    Tboard.x = (80-XMAX)/2;
    Tboard.y = 3;
    
    //Initialization
    initscr();
    start_color();
    noecho();
    cbreak;
    curs_set(FALSE);        
    keypad(stdscr, TRUE);  
    
    //Initialize color pairs
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);

    while (!exit)
    {
        key = 0;
        
        clear();
        mvprintw(1, (80 - 9)/2, "BOGGLE GAME");
        
        //Generate table of letters
        fill_grid(&Tboard, BOARD_SIZE);
        drawboard(&Tboard);
        //Main game start here
        gameEnded  = maingame(&Tboard);
        
        if (gameEnded  != 0)
        {
            attron(COLOR_PAIR(4));
            mvprintw(10, (80-7)/2, "GAME OVER");
            attron(A_BLINK);
            mvprintw(11, (80-12)/2, "FINAL SCORE %d", Tboard.score);
            attroff(A_BLINK);
            mvprintw(12, (80-29)/2, "Do you want to play again? (y/n)");
            attroff(COLOR_PAIR(4));
            
            while (key != 'n' && key != 'y')
            {
                key = getch();
                if (key == 'n'){
                    exit = 1;
                    free(Tboard.foundWord);
                }
            }
        } else {
            exit = 1;
        }
    }
    
    resetty();
    endwin();
 
    return 0;
}        
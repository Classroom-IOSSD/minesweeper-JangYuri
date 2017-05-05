#include <termios.h>
#include <stdio.h>

void initTermios(int echo);

/* Restore old terminal i/o settings */
void resetTermios(void); 

/* Read 1 character - echo defines echo mode */
char getch_(int echo);

/* Read 1 character without echo */
char getch(void);

/* Read 1 character with echo */
char getche(void);

void new_game();

void flag_mode();

void check_mode();

void end_of_game();

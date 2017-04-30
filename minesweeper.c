#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio.h"
#define MAX 10

// background color
#define KNRM  "\x1B[0m"
#define BRED  "\x1B[41m"
#define BGRN  "\x1B[42m"
#define BYEL  "\x1B[43m"
#define BBLU  "\x1B[44m"
#define BMAG  "\x1B[45m"
#define BCYN  "\x1B[46m"
#define BWHT  "\x1B[47m"
// text color
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


// global variables
// game table
unsigned char table_array[MAX][MAX];
// location of cursor
int x=0, y=0;
// default = 0, flag mode = 1, check mode = 2, end of game = 3, the end = 4
int game_mode=0;
// the number of the remaining mines
int nMines;

void direction_control(char direction){

	// arrow direction
	if(direction == '8') {
		// up
		y = (MAX + --y) % MAX;
	} else if(direction == '2') {
		// down
		y = ++y % MAX;
	} else if(direction == '4') {
		x = (MAX + --x) % MAX;
	} else if(direction == '6') {
		x = ++x % MAX;
	}

}

void game_mode_control(char inputMode){

	if(inputMode == 'f' || inputMode == 'F')
		game_mode = 1;

	else if(inputMode == 'c' || inputMode == 'C')
		game_mode = 2;
	
	else if(inputMode == 'n' || inputMode == 'N')
		game_mode = 0;
	
	else if(inputMode == 'q' || inputMode == 'Q')
		game_mode = 3;

}

/*This is a recursive function which uncovers blank cells while they are adjacent*/
int uncover_blank_cell(int row, int col) {
	int i, value;
	int rows[8], columns[8];

	if(table_array[row][col] != 0)
		return 0; // error

	table_array[row][col] += 10; // uncover current cell

	// Get position of adjacent cells of current cell
	rows[0] = row - 1;
	columns[0] = col + 1;
	rows[1] = row;
	columns[1] = col + 1;
	rows[2] = row + 1;
	columns[2] = col + 1;
	rows[3] = row - 1;
	columns[3] = col;
	rows[4] = row + 1;
	columns[4] = col;
	rows[5] = row - 1;
	columns[5] = col - 1;
	rows[6] = row;
	columns[6] = col - 1;
	rows[7] = row + 1;
	columns[7] = col - 1;

	for(i = 0; i < 8; i++) {
		value = table_array[rows[i]][columns[i]];

		if( (rows[i] >= 0 && rows[i] < MAX) && (columns[i] >= 0 && columns[i] < MAX) ) {    // to prevent negative index and out of bounds
			if(value > 0 && value <= 8)
				table_array[rows[i]][columns[i]] += 10;                                        // it is a cell with 1-8 number so we need to uncover
			else if(value == 0)
				uncover_blank_cell(rows[i], columns[i]);
		}

	}

	return 1; // success!
}

void print_table() {
	// clear screen
	system("clear");

	int i, j, value;
	for(i = 0; i < MAX; i++) {
		for(j = 0; j < MAX; j++) {
			if(x == j && y == i) {
				if(game_mode == 1) {
					printf("|%sF%s",BMAG,KNRM);
					continue;
				} else if(game_mode == 2) {
					printf("|%sC%s",BMAG,KNRM);
					continue;
				}

			}
			value = table_array[i][j];

			if((value >= 0 && value <= 8) || value == 0 || value == 99)
				printf("|X");
			else if(value == 10) // clean area
				printf("|%s%d%s",KCYN, value - 10,KNRM);
			else if(value == 11) // the number of near mine is 1
				printf("|%s%d%s",KYEL, value - 10,KNRM);
			else if(value > 11 && value <= 18) // the number of near mine is greater than 1
				printf("|%s%d%s",KRED, value - 10,KNRM);
			else if((value >= 20 && value <= 28) || value == 100)
				printf("|%sF%s",KGRN,KNRM);
			else
				printf("ERROR"); // test purposes

		}
		printf("|\n");
	}

	printf("cell values: 'X' unknown, '%s0%s' no mines close, '1-8' number of near mines, '%sF%s' flag in cell\n",KCYN,KNRM,KGRN,KNRM);
	if(game_mode == 1) {
		printf("Enter (select to put/remove Flag in cell), c (Check cell), n (New game), q (Exit game): ");
	} else if(game_mode == 2) {
		printf("Enter (select to check cell), f (put/remove Flag in cell), n (New game), q (Exit game): ");
	}


}


void new_game(){

	int i, j, row, col, value;
	int rows[8], columns[8];
    
    // set cursors' position to 0
    x = 0;
    y = 0;

	// set all cells to 0
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			table_array[i][j] = 0;

	srand (time(NULL));                        // random seed

	for(i = 0; i < nMines; i++) {
		/* initialize random seed: */

		row = rand() % 10;                    // it generates a integer in the range 0 to 9
		col = rand() % 10;

		// put mines
		if(table_array[row][col] != 99) {
			table_array[row][col] = 99;

			// Get position of adjacent cells of current cell
			rows[0] = row - 1;
			columns[0] = col + 1;
			rows[1] = row;
			columns[1] = col + 1;
			rows[2] = row + 1;
			columns[2] = col + 1;
			rows[3] = row - 1;
			columns[3] = col;
			rows[4] = row + 1;
			columns[4] = col;
			rows[5] = row - 1;
			columns[5] = col - 1;
			rows[6] = row;
			columns[6] = col - 1;
			rows[7] = row + 1;
			columns[7] = col - 1;

			for(j = 0; j < 8; j++) {
				value = table_array[rows[j]][columns[j]];
				if( (rows[j] >= 0 && rows[j] < MAX) && (columns[j] >= 0 && columns[j] < MAX) ) {    // to prevent negative index and out of bounds
					if(value != 99)                                                                // to prevent remove mines
						table_array[rows[j]][columns[j]] += 1;                                    // sums 1 to each adjacent cell
				}
			}

		} else {                            // to make sure that there are the properly number of mines in table
			i--;
			continue;
		}
	}

    game_mode = 2;

}

void flag_mode(){
    int value;

	do {
		print_table();
        
        char select;
        select = getch();
		// arrow select
		direction_control(select);

		if(select == '\n') {
			value = table_array[y][x];

			if (value == 99) {                // mine case
				table_array[y][x] += 1;
				nMines -= 1;                // mine found
			} else if(value >= 0 && value <= 8) {    // number of mines case (the next cell is a mine)
				table_array[y][x] += 20;
			} else if(value >= 20 && value <= 28) {
				table_array[y][x] -= 20;
			}

			if(nMines == 0){
				game_mode = 3;
				break;
			}
		}
        else if(select == 'c' || select == 'C' || select == 'n' || select == 'N' || select == 'q' || select == 'Q') {
            game_mode_control(select);
            break;
        }
        
	} while (game_mode == 1);

}

void check_mode(){
    int value;

	do {
		print_table();

        char select;
        select = getch();
		// arrow direction
		direction_control(select);

		if(select == '\n') {
			value = table_array[y][x];
			if(value == 0)                        // blank case
				uncover_blank_cell(y, x);
			else if(value == 99) {             // mine case
				game_mode = 3;
				break;
			}
			else if(value > 0 && value <= 8)    // number case (the next cell is a mine)
				table_array[y][x] += 10;

			//    break;
		}
        else if(select == 'f' || select == 'F' || select == 'n' || select == 'N' || select == 'q' || select == 'Q') {
            game_mode_control(select);
            break;
        }
        
	} while (game_mode == 2);

}

void end_of_game(){

	print_table();

	char ans;

	printf("\nGAME OVER\n");

	if(nMines == 0)
		printf("you won!!!!\n");
	else
		printf("BOOM! you LOOSE!\n");

	do {
		printf("Are you sure to exit? (y or n)? ");
		ans = getch();
		putchar('\n');
		if(ans == 'y' || ans == 'Y') {
			printf("See you next time!\n");
			game_mode = 4;
			break;
		} else if(ans == 'n' || ans == 'N') {
			game_mode = 0;
			break;
		}
		printf("Please answer y or n\n");
	} while(1);
}

int main(int argc, char *argv[]) {

	// the number of mines
	if(argc == 2) {
		nMines = atoi(argv[1]);
	}

	while(game_mode != 4){
	    nMines = 10;
		new_game();
		while(game_mode > 0){
			if(game_mode == 1){
			    flag_mode();
			}
			else if(game_mode == 2){
			    check_mode();
			}
			else if(game_mode == 3){
			    end_of_game();
			    break;
			}
        }
	}

	fflush(stdin);

	return 0;
}


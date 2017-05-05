#include "conio.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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

#define BIT(x) (0x01 << (x))
#define BITMASK_SET(x,y) ((x) |= (y))
#define BITMASK_EXTRACT(x,y) ((x) & (y))
#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
#define BITMASK_CHECK(x,y) (((x) & (y)) == (y))


const unsigned int FLAG_MASK = BIT(4);
const unsigned int UNCOVERED_MASK = BIT(5);
const unsigned int MINE_MASK = BIT(6);
const unsigned int NUM_MINE_MASK = 0x0f;

extern inline _Bool has_mine(unsigned int cell){
	return BITMASK_CHECK(cell, MINE_MASK);
}

extern inline void put_mine(unsigned int *cell){
	BITMASK_SET(*cell, MINE_MASK);
}

extern inline unsigned int num_mines(unsigned int cell){
	return BITMASK_EXTRACT(cell, NUM_MINE_MASK);
}

extern inline _Bool is_uncovered(unsigned int cell){
	return BITMASK_CHECK(cell, UNCOVERED_MASK);
}

extern inline _Bool is_flagged(unsigned int cell){
	return BITMASK_CHECK(cell, FLAG_MASK);
}

extern inline void put_flag(unsigned int *cell){
	BITMASK_SET(*cell, FLAG_MASK);
}

extern inline void unflagged(unsigned int *cell){
	BITMASK_CLEAR(*cell, FLAG_MASK);
}

extern inline void uncover(unsigned int *cell){
	BITMASK_SET(*cell, UNCOVERED_MASK);
}


// global variables
// game table
unsigned int table_array[MAX][MAX];
// location of cursor
int x=0, y=0;
// default = 0, flag mode = 1, check mode = 2, end of game = 3, the end = 4
extern int game_mode;
// the number of the remaining mines
extern int nMines;

void direction_control(char direction){

	// arrow direction
	if(direction == '8') {
		// up
		y--;
        y = (MAX + y) % MAX;
	} else if(direction == '2') {
		// down
        y++;
		y = y % MAX;
	} else if(direction == '4') {
        x--;
		x = (MAX + x) % MAX;
	} else if(direction == '6') {
        x++;
		x = x % MAX;
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

	if(num_mines(table_array[row][col]) != 0)
		return 0; // error

	uncover(&table_array[row][col]); // uncover current cell

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
		if(rows[i] >= 0 && columns[i] >= 0 && rows[i] < MAX && columns[i] < MAX) {    // to prevent negative index and out of bounds
			value = table_array[rows[i]][columns[i]];
			if(num_mines(value) > 0 && !is_flagged(value) && !is_uncovered(value))
				uncover(&table_array[rows[i]][columns[i]]);                                        // it is a cell with 1-8 number so we need to uncover
			else if(num_mines(value) == 0 && !is_flagged(value) && !is_uncovered(value))
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

			if((!is_uncovered(value)) && (!is_flagged(value)))
				printf("|X");
			else if(num_mines(value) == 0 && !is_flagged(value)) // clean area
				printf("|%s%d%s",KCYN, num_mines(value),KNRM);
			else if(num_mines(value) == 1 && !is_flagged(value)) // the number of near mine is 1
				printf("|%s%d%s",KYEL, num_mines(value),KNRM);
			else if(num_mines(value) >= 2 && num_mines(value) <= 8 && !is_flagged(value)) // the number of near mine is greater than 1
				printf("|%s%d%s",KRED, num_mines(value),KNRM);
			else if(is_flagged(value))
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
		if(!has_mine(table_array[row][col])) {
			put_mine(&table_array[row][col]);

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
                if(rows[j] < 0 || columns[j] < 0)
                        continue;
				value = table_array[rows[j]][columns[j]];
				if( (rows[j] >= 0 && rows[j] < MAX) && (columns[j] >= 0 && columns[j] < MAX) ) {    // to prevent negative index and out of bounds
					if(!has_mine(value))                                                                // to prevent remove mines
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

			if (has_mine(value)) { // mine case
                if(!is_flagged(value)){ //mine found
				    put_flag(&table_array[y][x]);
				    nMines -= 1;
                }
                else if(is_flagged(value)){
                    unflagged(&table_array[y][x]);
                    nMines += 1;
                }
			} else if(!is_flagged(value)) {    // number of mines case (the next cell is a mine)
				put_flag(&table_array[y][x]);
			} else if(is_flagged(value)) {
				unflagged(&table_array[y][x]);
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

		if(select == '\n' && !is_flagged(table_array[y][x]) && !is_uncovered(table_array[y][x])) {
			value = table_array[y][x];
			if(has_mine(value)) {             // mine case
				game_mode = 3;
				break;
			}
			else if(num_mines(value) == 0)                        // blank case
				uncover_blank_cell(y, x);
			else if(num_mines(value) > 0)    // number case (the next cell is a mine)
				uncover(&table_array[y][x]);

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

	if(nMines == 0) {
        printf("\n--- CONGRATUALTION ---\n");
		printf("you won!!!!\n");
    }
	else {
        printf("\n--- GAME OVER ---\n");
		printf("BOOM! you LOOSE!\n");
    }

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


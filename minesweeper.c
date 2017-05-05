#include "conio.h"
#include <stdlib.h>

int game_mode=0;
// the number of the remaining mines
int nMines;



int main(int argc, char *argv[]) {
	while(game_mode != 4){

	// the number of mines
		if(argc == 2) {
			nMines = atoi(argv[1]);
		}
        else 
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

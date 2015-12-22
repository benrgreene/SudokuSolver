#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "proto.h"

#define LINELEN 20

// Define funcs
void print_board();

int board[9][9];
int chunk;

int main(int argc, char **argv){
	// All the vars we need
	char buffer[LINELEN];
	int len;
	int line_on = 0;

	// We need a file pointer so that if a file is passed we can just read the file into the 
	// sudoku array. USER FRIENDLY YAY!
	FILE *fp = stdin;
	
	if(argc > 1){
		fp = fopen(argv[1], "r");
		if(fp == NULL){
			dprintf(2, "Oops, that file could not be found or opened. Either way, ya snookered me.\n");
			exit(127);
		}
	}else{
		dprintf(1, "Please enter the lines of the sudoku one at a time.");
		// Sorry if this comes across as.... churlish....
		dprintf(1, "This functionality hasn't been added in yet, sorry\n");
		exit(127);
	}

	// This is the sudoku map setup. We populate the board using a file or user input
	while(line_on < 9){
		if(argc == 1){
			dprintf(1, "%% ");
		}
		if (fgets(buffer, LINELEN, fp) != buffer){
			// End of line, kid
			break;
		}
		len = strlen(buffer);
		int p_on = 0;
		int a;
		for(a = 0; a < len; a++){
			char c = buffer[a];
			if(isdigit(c)){
				board[line_on][p_on] = c - '0';
				p_on++;
				if(p_on > 8){
					line_on ++;
					break;
				}
			}
		}	
	}
	
	// This is where we actually solve the sudoku
	// Loop while there are incomplete parts on the board.
	int big_change = 0;
	int big_change_f = 0;
	while(board_complete() == 0){
		if(big_change_f == 1){
			big_change_f = 1- break_rows();
			if(big_change_f == 1){
				big_change_f = 1- break_cols();
			}
			big_change_f = last_chance();
			if(big_change_f == 1){
				dprintf(1, "There was no possible correct solution found\n");
				break;
			}else{
				big_change = 1;
			}
		}else if(big_change == 0){
			big_change = -1;
		}else if(big_change == -1){
			big_change_f = 1;
			big_change = 0;
		}else{
			big_change = 0;
			big_change_f = 0;
		}
		
		// for each block
			// check each space if only one number can go there
			// check each space if a number has to go there
		// loop while things in block have happened

		chunk = 0;
		while(chunk < 9){
			int changes = 1;
			while(changes == 1){
				// reset the changes. If a change occurs,
				// then set to 1. That way we know a change
				// has ocured this cycle
				changes = 0;
				int x_1 = (chunk / 3) * 3;
				int y_1 = (chunk % 3) * 3;
				int x = 0;
				int y = 0;
				while(x < 3){
					y = 0;
					while(y < 3){
						if(big_change_f == 1 && break_candidate() == 1){
							// only come in here if there are only 2 open spots in the chunk
							int total_avail = 0;
							int high_found = 0;
							int on, temp;
							for(on = 1; on < 10; on++){
								temp = is_available(x_1+x, y_1+y, on);
								if(temp == 1){
									total_avail++;
									high_found = on;
								}
							}
							if(total_avail == 2){
								board[x_1+x][y_1+y] = high_found;
								big_change = 1;
								big_change_f = 0;
								changes = 1;
							}
						}
						// check if there is only one number available for the coords
						if(board[x_1+x][y_1+y] == 0){
							int temp = num_available(x_1+x, y_1+y);
							if(temp > 0){
								board[x_1+x][y_1+y] = temp;
								changes = 1;
								big_change = 1;
							}
						}
						// now check if something HAS to go here
						if(board[x_1+x][y_1+y] == 0){
							int num_on;
							//loop through all the numbers
							for(num_on = 1; num_on < 10; num_on++){
								int found = 0;
								int xt = 0;
								// If this ever becomes 0, move on to the next number
								int avail = 1;
								// If this ever becomes 1, move on
								int avail_else = 0;
								while(xt < 3){
									int yt = 0;	
									while(yt < 3){
										if(board[x_1+xt][y_1+yt] == 0){
											int flag = is_available(x_1+xt, y_1+yt, num_on);
											if(xt == x && yt == y){
												avail = flag;
											}else{
												avail_else += flag;
											}
										}
										yt++;
									}
									xt++;
								}
								// This checks the flags to see if the number HAS to go there
								if(avail == 1 && avail_else == 0){
									board[x_1+x][y_1+y] = num_on;
									changes = 1;
									big_change = 1;
									break;
								}
							}
						}
						y++;
					}
					x++;
				}
			}
			chunk++;
		}
	}
	print_board();
}

// Prints the board
void print_board(){
	int b;
	int c;
	dprintf(1, "-----------------------------------------\n\n");
	for(b = 0; b < 9; b++){
		for(c = 0; c < 9; c++){
			dprintf(1, "%d   ", board[b][c]);
			if(c % 3 == 2){
				dprintf(1, "    ");
			}
		}
		dprintf(1, "\n");
		if(b % 3 == 2){
			dprintf(1, "\n");
		}
	}
	dprintf(1, "-----------------------------------------\n\n");}
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "proto.h"

// Returns 1 if board is complete, 0 otherwise
int board_complete(){
	int to_return = 1;
	int a, b;
	for(a = 0; a < 9; a++){
		for(b = 0; b < 9; b++){
			if(board[a][b] == 0){
				return 0;
			}
		}
	}
	return to_return;
}

// this returns the either the only number that
// can be placed in a cell, or -1 if there are
// multiple possibilities
int num_available(int pos_x, int pos_y){
	int to_return = 0;
	int a;
	for(a = 1; a < 10; a++){
		int avail = is_available(pos_x, pos_y, a);
		if(avail == 1){
			// if to_return is 0, no other value has been found, set return to that num
			// otherwise, there are mulitple possibilities.
			if(to_return == 0){
				to_return = a;
			}else{
				to_return = -1;
				break;
			}
		}
	}
	if(to_return == 0){
		to_return = -1;
	}
	return to_return;
}

// returns 1 if the number passed can be placed in a given pos
int is_available(int pos_x, int pos_y, int num){
	int to_return = 1;
	// check chunk
	int x_1 = (chunk / 3) * 3;
	int y_1 = (chunk % 3) * 3;
	int x = 0;
	int y = 0;
	while(x < 3){
		y = 0;
		while(y < 3){
			if((x_1 + x) != pos_x || (y_1 + y) != pos_y){
				int temp = board[x_1 + x][y_1 + y];
				if(temp == num){
					return 0;
				}
			}
			y++;
		}
		x++;
	}
	// check row
	int a;
	for(a = 0; a < 9; a++){
		if(board[pos_x][a] == num){
			to_return = 0;
			break;
		}
	}
	
	// check col
		for(a = 0; a < 9; a++){
		if(board[a][pos_y] == num){
			to_return = 0;
			break;
		}
	}
	return to_return;
}

// this returns if there are two blank spots in a chunk
int break_candidate(){
	int x_1 = (chunk / 3) * 3;
	int y_1 = (chunk % 3) * 3;
	int x = 0;
	int y = 0;
	int open = 0;
	while(x < 3){
		y = 0;
		while(y < 3){
			open += (board[x_1+x][y_1+y] == 0);
			y++;
		}
		x++;
	}
	return (open == 2);
}

// checks if there is a row with only two open spots
// returns 1 if it broke a cycle
int break_rows(){
	int x;
	int y;
	int avail = 0;
	for(x = 0; x < 9; x++){
		avail = 0;
		for(y = 0; y < 9; y++){
			avail += (board[x][y] == 0);
		}
		if(avail == 2){
			for(y = 0; y < 9; y++){
				if(board[x][y] == 0){
					int a;	
					chunk = (x / 3)*3 + (y/3);
					for(a = 1; a < 10; a++){
						if(is_available(x, y, a)){
							board[x][y] = a;
							return 1;
						}
					}
				}
			}	
		}
	}
	return 0;
}

// checks if there is a col with only two open spots
// returns 1 if it broke a cycle
int break_cols(){
	int x;
	int y;
	int avail = 0;
	for(y = 0; y < 9; y++){
		avail = 0;
		for(x = 0; x < 9; x++){
			avail += (board[x][y] == 0);
		}
		if(avail == 2){
			for(y = 0; y < 9; y++){
				if(board[x][y] == 0){
					int a;
					chunk = (x / 3)*3 + (y/3);
					for(a = 1; a < 10; a++){
						if(is_available(x, y, a)){
							board[x][y] = a;
							return 1;
						}
					}
				}
			}	
		}
	}
	return 0;
}

int last_chance(){
	// super huge loop. I dunno, just throw a random number somewhere. 
	int x;
	int y;
	int tr;
	for(x = 0; x < 9; x++){
		for(y = 0; y < 9; y++){
			if(board[x][y] == 0){
				int c;
				chunk = (x / 3)*3 + (y/3);
				for(c = 1; c < 10; c++){
					if(is_available(x,y,c) == 1){
						board[x][y] = c;
						return 0;
					}
				}
			}
		}		
	}
	return 1;
}
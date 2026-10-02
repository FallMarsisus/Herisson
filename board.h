/* board.h
--> Used to define the main game structure, the main functions used with it, 
    along with the functions used to print the board
*/


#ifndef BOARD_H
#define BOARD_H

#include "board.h"
#include "primitives.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct board_t {
	char*** board; // each element of the board is a pile of the hedgehogs in it.
	int** n_hedge; // at each position, keep track of the number of hedgehogs
	bool** is_trapped; 
	int n_lines; 
	int n_rows; 
} board_t;


board_t board_init();
void board_push(board_t* b, int line, int row, char ctn);
char board_pop(board_t* b, int line, int row);
int board_height(board_t* b, int line, int row);
char board_top(board_t* b, int line, int row);
char board_peek(board_t* b, int line, int row, int pos); // pos=0 => top
void cell_print(board_t* b, int line, int row, int slice);
void board_print(board_t* b, int highlighted_line); // hl_line=-1 => nothing selected


#endif
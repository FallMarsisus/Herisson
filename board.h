/* board.h
--> Used to define the main game structure, the main functions used with it,
	along with the functions used to print the board
*/

#ifndef BOARD_H
#define BOARD_H

#include "primitives.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct board_t
{
	char ***board; // each element of the board is a pile of the hedgehogs in it.
	int **n_hedge; // at each position, keep track of the number of hedgehogs
	bool **is_trapped;
	int *n_finished; // number of hedgehogs which finished the run per TEAM
	int n_lines;
	int n_rows;
	int player ;
	bool game_is_finished ;
} board_t;

board_t board_init(void);
void board_free(board_t b);
void board_push(board_t *b, int line, int row, char ctn);
char board_pop(board_t *b, int line, int row);
int board_height(board_t *b, int line, int row);
char board_top(board_t *b, int line, int row);
char board_peek(board_t *b, int line, int row, int pos); // pos=0 => top


#endif
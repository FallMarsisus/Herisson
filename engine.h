/* board.h
--> Used to define the main game structure, the main functions used with it,
	along with the functions used to print the board
*/

#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"
#include "primitives.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {int x1 ; int y1 ; int x2 ; int y2 ;} move ;

int dice() ;
void play_row(board_t* b) ;
void play_line(board_t* b) ;
move scan_move(board_t* b) ;

#endif
/* engine.h
--> Main logic and input functions
*/

#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"
#include "primitives.h"
#include "ui.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {int x1 ; int y1 ; int x2 ; int y2 ;} move ;
enum step {VERTICAL, HORIZONTAL} ;
typedef enum step step ;

int min(int a, int b) ;
int dice(void) ;
int play_turn(board_t* b) ;
move scan_move(board_t* b, step s, int dice_roll) ;

#endif
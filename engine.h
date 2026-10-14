/* engine.h
--> Main logic and input functions
*/

#ifndef ENGINE_H
#define ENGINE_H

#include "all_move.h"
#include "board.h"
#include "primitives.h"
#include "ui.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int min(int a, int b);
int dice(void);
int play_turn(board_t *b);
bool test_possible_move(board_t *b, step s, int dice_roll);
void scan_move(board_t *b, step s, int dice_roll, move* m);

#endif
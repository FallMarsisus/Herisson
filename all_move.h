/* all_move.h
--> extension: the user can move a hedgehog diagonally and backwards
*/

#ifndef ALL_MOVE_H
#define ALL_MOVE_H

#include "board.h"
#include "primitives.h"
#include "ui.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  int x1;
  int y1;
  int x2;
  int y2;
} move;
typedef enum { VERTICAL, HORIZONTAL } step;

typedef enum { CLASSIC, ALL_MOVE } am_mode;

void am_mode_init(am_mode m);
int valabs(int a);
bool test_input_is_correct(board_t *b, unsigned int x_atoi,
                           unsigned int y_atoi);
bool test_new_cell_is_ok(move *m);
void am_horizontal(board_t *b, move *m, unsigned int *x_atoi,
                   unsigned int *y_atoi);
void am_vertical(board_t *b, move *m, unsigned int *x_atoi,
                 unsigned int *y_atoi);

/*void input_to_coordinate(char* input, char* x_input, char* y_input, step s) ;
void test_input_is_correct(board_t* b, unsigned int x_atoi, unsigned int y_atoi)
; move am_scan_move(board_t *b, step s, int dice_roll);*/

#endif
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

int min(int a, int b);
void am_mode_init(am_mode m);
bool test_possible_move(board_t *b, step s, int dice_roll);
int am_input_2_coordinates(unsigned int *x_atoi, unsigned int *y_atoi, step s,
                       int dice_roll);
bool test_input_is_correct(board_t *b, unsigned int x_atoi,
                           unsigned int y_atoi);
int test_move_is_ok(move *m);
int valabs(int a);
void am_assign_move_horizontal(board_t *b, move *m, unsigned int *x_atoi,
                   unsigned int *y_atoi, step s);
int am_assign_move_vertical(board_t *b, move *m, unsigned int *x_atoi,
                 unsigned int *y_atoi, step s, int dice_roll);

/*void input_to_coordinate(char* input, char* x_input, char* y_input, step s) ;
void test_input_is_correct(board_t* b, unsigned int x_atoi, unsigned int y_atoi)
; move am_scan_move(board_t *b, step s, int dice_roll);*/

#endif
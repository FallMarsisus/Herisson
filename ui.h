#ifndef UI_H
#define UI_H


#ifdef ENABLE_GUI // to enable, compile with gui enabled

#include <raylib.h>

#endif // ENABLE_GUI

#include "board.h"
#include <assert.h>

typedef enum {
    UI_MODE_UNSET,
    UI_MODE_TERM,
    UI_MODE_GRAPHIC
} ui_mode; 


void ui_init(ui_mode m);
void ui_free(void);
void ui_printf(const char* format, ...);
void ui_scanf(const char* format, ...);

// Main functions to use to draw the board
void ui_render_cell(board_t* b, int line, int row, int slice);
void ui_render_board(board_t* b, int highlighted_line);


// Private functions used to target a specific interface.

void gui_render_cell(board_t* b, int line, int row);
void gui_render_board(board_t* b, int highlighted_line);

void term_render_cell(board_t*b, int line, int row, int slice);
void term_render_board(board_t* b, int highlighted_line);


#endif // UI_H
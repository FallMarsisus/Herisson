#include "ui.h"

ui_mode mode; 

void ui_init(ui_mode m)
{
    mode = m;

    if (m == UI_MODE_GRAPHIC)
    {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "HedgeHog Game - UI Version");
        SetTargetFPS(60);
    }
}

void ui_free(void)
{
    assert(mode != UI_MODE_UNSET);
    CloseWindow();
}

/// Render a cell, slice is only used when using the terminal, use -1 if necessary
void ui_render_cell(board_t *b, int line, int row, int slice)
{
    assert(mode != UI_MODE_UNSET);

    if (mode == UI_MODE_GRAPHIC)
        gui_render_cell(b, line, row);
    else
        term_render_cell(b, line, row, slice);
}

void ui_render_board(board_t *b, int highlighted_line)
{
    assert(mode != UI_MODE_UNSET);

    if (mode == UI_MODE_GRAPHIC)
        gui_render_board(b, highlighted_line);
    else
        term_render_board(b, highlighted_line);
}

void gui_render_cell(board_t *b, int line, int row)
{
}



void gui_render_board(board_t *b, int highlighted_line)
{
}


void term_render_cell(board_t *b, int line, int row, int slice)
{
    int n_hedge = b->n_hedge[line][row];
    bool is_trapped = b->is_trapped[line][row];

    char fst_line[4] = "   \0";
    char snd_line[4] = "   \0";
    char thd_line[4] = "   \0";

    switch (n_hedge)
    {
    case 4:
        for (int i = 0; i < 3; i++)
            snd_line[i] = b->board[line][row][2 - i] - ('A' - 'a');
        break;
    case 3:
        snd_line[0] = b->board[line][row][1] - ('A' - 'a');
        snd_line[1] = ' ';
        snd_line[2] = b->board[line][row][0] - ('A' - 'a');
        break;
    case 2:
        for (int i = 0; i < 3; i++)
            snd_line[i] = b->board[line][row][0] - ('A' - 'a');
        break;
    case 1:
        for (int i = 0; i < 3; i++)
            snd_line[i] = b->board[line][row][0];
        break;
    default: // More than 4, only show first 4
        for (int i = n_hedge - 2; i > n_hedge - 6; i--)
            snd_line[n_hedge - 2 - i] = b->board[line][row][i] - ('A' - 'a');
        break;
    }
    if (n_hedge > 0)
    {
        for (int i = 0; i < 3; i++)
            fst_line[i] = b->board[line][row][n_hedge - 1];
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            fst_line[i] = ' ';
            snd_line[i] = ' ';
        }
    }

    if (is_trapped)
    {
        for (int i = 0; i < 3; i++)
            thd_line[i] = '^';
    }
    else
    {
        thd_line[0] = '-';
        thd_line[1] = n_hedge == 0 ? '-' : n_hedge + '0';
        thd_line[2] = '-';
    }

    snd_line[3] = '\0';
    fst_line[3] = '\0';
    thd_line[3] = '\0';

    switch (slice)
    {

    case 0:
        printf(" %s ",
               is_trapped ? "vvv" : "---");
        break;
    case 1:
        printf("%c%s%c",
               is_trapped ? '>' : '|',
               fst_line,
               is_trapped ? '<' : '|');
        break;
    case 2:
        printf("%c%s%c",
               is_trapped ? '>' : '|',
               snd_line,
               is_trapped ? '<' : '|');
        break;
    case 3:
        printf(" %s ",
               thd_line);
        break;
    default:
        break;
    }
}




void term_render_board(board_t *b, int highlighted_line)
{

   // First row : show indices
   printf("      ");
   for(int i = 0; i < b->n_rows; i++) {
        printf("y     ");
   }
   printf("\n      ");
   for(int i = 0; i < b->n_rows; i++) {
        printf(" %d    ", i+1);
   }
   printf("\n");


   // ... then the board
    for (int i = 0; i < b->n_lines * 4; i++)
    {

        switch(i%4) {
                case 0:
                    printf("     ");
                    break;
                case 1:
                    printf(" x   ");
                    break;
                case 2:
                    printf("  %d  ", i/4 + 1);
                    break;
                case 3: 
                    printf("     ");
                    break; 
                default:
                    break;
            }


        for (int j = 0; j < b->n_rows; j++)
        {
            

            ui_render_cell(b, i / 4, j, i % 4 );
            printf(" ");
        }
        i % 4 == 3 ? printf("\n\n") : printf("\n"); // Print a blank between each line
    }
}

#include "ui.h"

ui_mode mode;
static board_t *last_board = NULL;
static int gui_cursor = 5; 

void ui_init(ui_mode m)
{
    mode = m;

#ifdef ENABLE_GUI
    if (m == UI_MODE_GRAPHIC)
    {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "HedgeHog Game - UI Version");
        SetTargetFPS(60);
    }
#endif
}

void ui_free(void)
{
    assert(mode != UI_MODE_UNSET);

#ifdef ENABLE_GUI
    CloseWindow();
#endif
}

void ui_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    if (mode == UI_MODE_TERM)
    {
        // Same use as printf
        vprintf(format, args);
    }
    else
    {
#ifdef ENABLE_GUI
        char buffer[512];
        vsnprintf(buffer, sizeof(buffer), format, args);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        ui_render_board(last_board, -1);
        DrawText(buffer, 10, gui_cursor, 12, BLACK);
        gui_cursor += 14;
        EndDrawing();

#else
        printf("GUI is currently disabled on compilation, please compile with 'make gui' to enable it.\n");
#endif
    }

    va_end(args);
}

int ui_scanf(const char *format, ...) {
    if (mode == UI_MODE_TERM) {
        va_list args;
        va_start(args, format);
        int ret = vscanf(format, args);
        va_end(args);
        return ret;
    }

#ifdef ENABLE_GUI
    else if (mode == UI_MODE_GRAPHIC) {
        char input_text[128] = {0};
        int letter_count = 0;
        bool valid_input = false;
        bool show_error = false;
        int ret = 0;

        /* Disposition centrée dans la moitié GAUCHE de l'écran */
        float left_panel_w = SCREEN_WIDTH / 2.0f;
        float box_w = left_panel_w - 60.0f;
        float box_h = 240.0f;
        Rectangle box = { 30.0f, (SCREEN_HEIGHT - box_h) / 2.0f, box_w, box_h };
        Rectangle text_box = { box.x + 20.0f, box.y + 80.0f, box_w - 40.0f, 40.0f };
        Rectangle btn_ok = { box.x + (box_w - 120.0f) / 2.0f, box.y + 140.0f, 120.0f, 40.0f };

        int frames_counter = 0;

        /* La boucle ne s'arrête que si la saisie correspond bien au format attendu */
        while (!valid_input) {
            if (WindowShouldClose()) {
                return EOF;
            }

            frames_counter++;

            /* 1. Capture des caractères */
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 126) && (letter_count < (int)sizeof(input_text) - 1)) {
                    input_text[letter_count] = (char)key;
                    input_text[letter_count + 1] = '\0';
                    letter_count++;
                    show_error = false;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (letter_count > 0) {
                    letter_count--;
                    input_text[letter_count] = '\0';
                    show_error = false;
                }
            }

            Vector2 mouse = GetMousePosition();
            bool mouse_on_btn = CheckCollisionPointRec(mouse, btn_ok);
            bool submit = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) ||
                          (mouse_on_btn && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));

            if (submit) {
                if (letter_count > 0) {
                    va_list args;
                    va_start(args, format);
                    ret = vsscanf(input_text, format, args);
                    va_end(args);

                    if (ret > 0) {
                        valid_input = true;
                    } else {
                        show_error = true; 
                    }
                } else {
                    show_error = true;
                }
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            if (last_board != NULL) {
                for (int i = 0; i < last_board->n_lines; i++) {
                    for (int j = 0; j < last_board->n_rows; j++) {
                        gui_render_cell(last_board, i, j);
                    }
                }
            }

            DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, LIGHTGRAY);

            DrawRectangleRec(box, WHITE);
            DrawRectangleLinesEx(box, 2, DARKGRAY);
            DrawText("Saisie requise :", box.x + 20, box.y + 25, 20, BLACK);

            DrawRectangleRec(text_box, RAYWHITE);
            DrawRectangleLinesEx(text_box, 1.5f, show_error ? RED : DARKBLUE);
            DrawText(input_text, text_box.x + 10, text_box.y + 10, 20, DARKGRAY);

            if ((frames_counter / 30) % 2 == 0) {
                int text_w = MeasureText(input_text, 20);
                DrawText("|", text_box.x + 10 + text_w, text_box.y + 8, 22, DARKBLUE);
            }

            if (show_error) {
                DrawText("Format invalide, recommencez !", box.x + 20, box.y + 195, 16, RED);
            }

            DrawRectangleRec(btn_ok, mouse_on_btn ? SKYBLUE : BLUE);
            DrawRectangleLinesEx(btn_ok, 1, DARKBLUE);
            DrawText("OK", btn_ok.x + 45, btn_ok.y + 10, 20, WHITE);

            EndDrawing();
        }

        return ret;
    }
#endif
    return 0;
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
#ifdef ENABLE_GUI

    DrawRectangle(SCREEN_WIDTH / 2 + 25 + (line) * (SCREEN_WIDTH / 2 - 50) / b->n_lines, 25 + (row) * (SCREEN_HEIGHT - 50) / b->n_rows, (SCREEN_WIDTH / 2 - 50) / b->n_lines - 10, (SCREEN_HEIGHT - 50) / b->n_rows - 10, BLACK);
#else
    printf("GUI is currently disabled on compilation, please compile with 'make gui' to enable it.\n");
#endif
}

void gui_render_board(board_t *b, int highlighted_line)
{
#ifdef ENABLE_GUI

    last_board = b;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int i = 0; i < b->n_lines; i++)
    {
        for (int j = 0; j < b->n_rows; j++)
        {
            ui_render_cell(b, i, j, -1);
        }
    }
    EndDrawing();
#else
    printf("GUI is currently disabled on compilation, please compile with 'make gui' to enable it.\n");
#endif
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
    for (int i = 0; i < b->n_rows; i++)
    {
        printf("y     ");
    }
    printf("\n      ");
    for (int i = 0; i < b->n_rows; i++)
    {
        printf(" %d    ", i + 1);
    }
    printf("\n");

    // ... then the board
    for (int i = 0; i < b->n_lines * 4; i++)
    {

        switch (i % 4)
        {
        case 0:
            printf("     ");
            break;
        case 1:
            printf(" x   ");
            break;
        case 2:
            printf("  %d  ", i / 4 + 1);
            break;
        case 3:
            printf("     ");
            break;
        default:
            break;
        }

        for (int j = 0; j < b->n_rows; j++)
        {

            ui_render_cell(b, i / 4, j, i % 4);
            printf(" ");
        }
        i % 4 == 3 ? printf("\n\n") : printf("\n"); // Print a blank between each line
    }
}

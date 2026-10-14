#include "ui.h"

ui_mode mode;

#ifdef ENABLE_GUI
static board_t *last_board =
    NULL; // used to easily redraw the board on user interaction
int last_highlight = -1;
char history[1024]; // used to show text to the user
Color list_teams[] = {
    RED, BLUE, GREEN,
    YELLOW}; // Colors of the teams, in order of their ID (A, B, C, D)
#endif

void ui_init(ui_mode m) {
  mode = m;

#ifdef ENABLE_GUI
  if (m == UI_MODE_GRAPHIC) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "HedgeHog Game - UI Version");
    SetTargetFPS(60);
  }
#endif
}

#ifdef ENABLE_GUI
void show_team_colors() {
  DrawText("Colors: ", 30, SCREEN_HEIGHT - 36, 20, BLACK);
  for (int i = 0; i < N_PLAYERS; i += 1) {
    DrawRectangle(110 + 50 * i, (SCREEN_HEIGHT - 46), 40, 40, list_teams[i]);
    char team[2] = {i + 'A', '\0'};
    DrawText(team, 123 + 50 * i, SCREEN_HEIGHT - 36, 20, WHITE);
  }
}

#endif

void ui_free(void) {
  assert(mode != UI_MODE_UNSET);

#ifdef ENABLE_GUI
  CloseWindow();
#endif
}

#ifdef ENABLE_GUI
void show_history() { DrawText(history, 10, 10, 20, BLACK); }

void flush_history() { history[0] = '\0'; }
#endif

void ui_printf(const char *format, ...) {
  va_list args;
  va_start(args, format);

  if (mode == UI_MODE_TERM) {
    // Same use as printf
    vprintf(format, args);
  } else {
#ifdef ENABLE_GUI
    char buffer[512];
    vsnprintf(buffer, sizeof(buffer), format, args);

    strcat(history, buffer);

#else
    printf("GUI is currently disabled on compilation, please compile with "
           "'make gui' to enable it.\n");
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
    Rectangle box = {30.0f, (SCREEN_HEIGHT - box_h) - 50, box_w, box_h};
    Rectangle text_box = {box.x + 20.0f, box.y + 80.0f, box_w - 40.0f, 40.0f};
    Rectangle btn_ok = {box.x + (box_w - 120.0f) / 2.0f, box.y + 140.0f, 120.0f,
                        40.0f};

    int frames_counter = 0;

    while (!valid_input) {
      if (WindowShouldClose()) {
        exit(0);
        return EOF;
      }

      frames_counter += 1;

      int key = GetCharPressed();
      while (key > 0) {
        if ((key >= 32) && (key <= 126) &&
            (letter_count < (int)sizeof(input_text) - 1)) {
          input_text[letter_count] = (char)key;
          input_text[letter_count + 1] = '\0';
          letter_count += 1;
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

      gui_render_board(last_board, last_highlight);
      show_history();

      show_team_colors();

      DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, LIGHTGRAY);

      DrawRectangleRec(box, WHITE);
      DrawRectangleLinesEx(box, 2, DARKGRAY);
      DrawText("Input :", box.x + 20, box.y + 25, 20, BLACK);

      DrawRectangleRec(text_box, RAYWHITE);
      DrawRectangleLinesEx(text_box, 1.5f, show_error ? RED : DARKBLUE);
      DrawText(input_text, text_box.x + 10, text_box.y + 10, 20, DARKGRAY);

      if ((frames_counter / 30) % 2 == 0) {
        int text_w = MeasureText(input_text, 20);
        DrawText("|", text_box.x + 10 + text_w, text_box.y + 8, 22, DARKBLUE);
      }

      if (show_error) {
        DrawText("Invalid format, restart.", box.x + 20, box.y + 195, 16, RED);
      }

      DrawRectangleRec(btn_ok, mouse_on_btn ? SKYBLUE : BLUE);
      DrawRectangleLinesEx(btn_ok, 1, DARKBLUE);
      DrawText("OK", btn_ok.x + 45, btn_ok.y + 10, 20, WHITE);

      EndDrawing();
    }
    flush_history();

    return ret;
  }
#endif
  return 0;
}

void ui_show(board_t* b) {
  if (mode == UI_MODE_TERM) {
    int c;
    if(!b -> game_is_finished){
      printf("Press [enter] to continue.\n");
    }
    while ((c = getchar()) != '\n' && c != EOF)
      ;

    return;
  }

  if (mode == UI_MODE_GRAPHIC) {
#ifdef ENABLE_GUI

    float left_panel_w = SCREEN_WIDTH / 2.0f;
    float box_w = left_panel_w - 60.0f;
    float box_h = 60.0f;

    Rectangle box = {30.0f, (SCREEN_HEIGHT - box_h) - 50, box_w, box_h};
    Rectangle btn_ok = {box.x + (box_w)-140.0f, box.y + 10.0f, 120.0f, 40.0f};

    bool submit = false;
    while (!submit) {
      if (WindowShouldClose()) {
        exit(0);
        return;
      }

      Vector2 mouse = GetMousePosition();
      bool mouse_on_btn = CheckCollisionPointRec(mouse, btn_ok);
      submit = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) ||
               (mouse_on_btn && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));

      BeginDrawing();
      ClearBackground(RAYWHITE);

      gui_render_board(last_board, last_highlight);
      show_history();

      show_team_colors();

      DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, LIGHTGRAY);

      DrawRectangleRec(box, WHITE);
      DrawRectangleLinesEx(box, 2, DARKGRAY);
      DrawText("Press [OK] to continue.", box.x + 18, box.y + 25, 20, BLACK);

      DrawRectangleRec(btn_ok, mouse_on_btn ? LIGHTGRAY : GRAY);
      DrawRectangleLinesEx(btn_ok, 1, DARKBLUE);
      DrawText("OK", btn_ok.x + 45, btn_ok.y + 10, 20, WHITE);

      EndDrawing();
    }

    flush_history();

#endif
  }
}

/// Render a cell, slice is only used when using the terminal, use -1 if
/// necessary
void ui_render_cell(board_t *b, int line, int row, int slice) {
  assert(mode != UI_MODE_UNSET);

  if (mode == UI_MODE_GRAPHIC)
#ifdef ENABLE_GUI
    gui_render_cell(b, line, row);
#else
    printf("GUI is currently diabled on compilation, please enable it to use "
           "GUI\n");
#endif
  else
    term_render_cell(b, line, row, slice);
}

void ui_render_board(board_t *b, int highlighted_line) {
  assert(mode != UI_MODE_UNSET);

  if (mode == UI_MODE_GRAPHIC)
#ifdef ENABLE_GUI
    gui_render_board(b, highlighted_line);
#else
    printf("GUI is currently diabled on compilation, please enable it to use "
           "GUI\n");
#endif
  else
    term_render_board(b, highlighted_line);
}

#ifdef ENABLE_GUI
void gui_render_cell(board_t *b, int line, int row) {

  int cell_width = (SCREEN_WIDTH / 2 - 50) / b->n_rows - 5;
  int cell_height = (SCREEN_HEIGHT - 60) / b->n_lines - 10;
  int cell_x =
      SCREEN_WIDTH / 2 + 25 + (row) * (SCREEN_WIDTH / 2 - 50) / b->n_rows;
  int cell_y = 30 + (line) * (SCREEN_HEIGHT - 60) / b->n_lines;
  if (board_height(b, line, row) == 0) {
    DrawRectangle(cell_x, cell_y, cell_width, cell_height, BLACK);
  } else {
    Color a = list_teams[board_top(b, line, row) - 'A'];
    DrawRectangle(cell_x, cell_y, cell_width, cell_height, a);

    int bh = board_height(b, line, row);
    for (int i = 1; i < bh; i += 1) {
      Color a = list_teams[board_peek(b, line, row, bh - 1 - i) - 'A'];
      int y = cell_y + cell_height - bh * 10 + i * 10;
      DrawRectangle(cell_x, y, cell_width, 10, a);

      DrawLine(cell_x, y, cell_width + cell_x, y, BLACK);
    }
  }

  if (b->is_trapped[line][row]) {
    int size = 3;
    for (int i = 0; i < cell_width / size; i += 1) {
      for (int j = 0; j < cell_height / size; j += 1) {
        if ((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0))
          DrawRectangle(cell_x + i * size, cell_y + j * size, size, size,
                        LIGHTGRAY);
      }
    }
  }
}

#endif

#ifdef ENABLE_GUI
void gui_render_board(board_t *b, int highlighted_line) {

  last_board = b;
  for (int i = 0; i < b->n_rows; i += 1) {
    char yi[8];
    sprintf(yi, "%d", i + 1);
    DrawText(yi,
             SCREEN_WIDTH / 2 + 25 + (i) * (SCREEN_WIDTH / 2 - 50) / b->n_rows,
             10, 15, BLACK);
  }

  for (int j = 0; j < b->n_lines; j += 1) {
    if (j == highlighted_line) {
      DrawRectangle(SCREEN_WIDTH / 2 + 5,
                    25 + (j) * (SCREEN_HEIGHT - 60) / b->n_lines,
                    (SCREEN_WIDTH - 60) / 2,
                    (1) * (SCREEN_HEIGHT - 60) / b->n_lines, YELLOW);
      last_highlight = highlighted_line;
    }
    char xi[8];
    sprintf(xi, "%d", j + 1);
    DrawText(xi, SCREEN_WIDTH / 2 + 10,
             30 + (j) * (SCREEN_HEIGHT - 60) / b->n_lines, 15, BLACK);
  }

  for (int i = 0; i < b->n_lines; i += 1) {

    for (int j = 0; j < b->n_rows; j += 1) {
      ui_render_cell(b, i, j, -1);
    }
  }
}
#endif

void term_render_cell(board_t *b, int line, int row, int slice) {
  int n_hedge = b->n_hedge[line][row];
  bool is_trapped = b->is_trapped[line][row];

  char fst_line[4] = "   \0";
  char snd_line[4] = "   \0";
  char thd_line[4] = "   \0";

  switch (n_hedge) {
  case 4:
    for (int i = 0; i < 3; i += 1)
      snd_line[i] = b->board[line][row][2 - i] - ('A' - 'a');
    break;
  case 3:
    snd_line[0] = b->board[line][row][1] - ('A' - 'a');
    snd_line[1] = ' ';
    snd_line[2] = b->board[line][row][0] - ('A' - 'a');
    break;
  case 2:
    for (int i = 0; i < 3; i += 1)
      snd_line[i] = b->board[line][row][0] - ('A' - 'a');
    break;
  case 1:
    for (int i = 0; i < 3; i += 1)
      snd_line[i] = b->board[line][row][0];
    break;
  default: // More than 4, only show first 4
    for (int i = n_hedge - 2; i > n_hedge - 6; i--)
      snd_line[n_hedge - 2 - i] = b->board[line][row][i] - ('A' - 'a');
    break;
  }
  if (n_hedge > 0) {
    for (int i = 0; i < 3; i += 1)
      fst_line[i] = b->board[line][row][n_hedge - 1];
  } else {
    for (int i = 0; i < 3; i += 1) {
      fst_line[i] = ' ';
      snd_line[i] = ' ';
    }
  }

  if (is_trapped) {
    for (int i = 0; i < 3; i += 1)
      thd_line[i] = '^';
  } else {
    thd_line[0] = '-';
    thd_line[1] = n_hedge == 0 ? '-' : n_hedge + '0';
    thd_line[2] = '-';
  }

  snd_line[3] = '\0';
  fst_line[3] = '\0';
  thd_line[3] = '\0';

  switch (slice) {

  case 0:
    printf(" %s ", is_trapped ? "vvv" : "---");
    break;
  case 1:
    printf("%c%s%c", is_trapped ? '>' : '|', fst_line, is_trapped ? '<' : '|');
    break;
  case 2:
    printf("%c%s%c", is_trapped ? '>' : '|', snd_line, is_trapped ? '<' : '|');
    break;
  case 3:
    printf(" %s ", thd_line);
    break;
  default:
    break;
  }
}

void term_render_board(board_t *b, int highlighted_line) {

  // First row : show indices
  printf("      ");
  for (int i = 0; i < b->n_rows; i += 1) {
    printf("y     ");
  }
  printf("\n      ");
  for (int i = 0; i < b->n_rows; i += 1) {
    printf(" %d    ", i + 1);
  }
  printf("\n");

  // ... then the board
  for (int i = 0; i < b->n_lines * 4; i += 1) {
    char selected = highlighted_line == i / 4 ? '>' : ' ';

    switch (i % 4) {
    case 0:
      printf("%c    ", selected);
      break;
    case 1:
      printf("%c x  ", selected);
      break;
    case 2:
      printf("%c  %d ", selected, i / 4 + 1);
      break;
    case 3:
      printf("%c    ", selected);
      break;
    default:
      break;
    }

    for (int j = 0; j < b->n_rows; j += 1) {

      ui_render_cell(b, i / 4, j, i % 4);
      printf(" ");
    }
    i % 4 == 3 ? printf("\n\n")
               : printf("\n"); // Print a blank between each line
  }
}

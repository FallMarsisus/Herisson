#include "engine.h"

int dice(void) { return rand() % GAME_SIZE_Y + 1; }

int min(int a, int b) { return (a < b ? a : b); }

// recieves the inputs, changes the board ("setter")
int play_turn(board_t *b) {
  int dice_roll = dice();
  static int winning_player = 0;
  move *m = malloc(sizeof(move));
  for (int i = 0; i < 2; i += 1) {
    scan_move(b, i, dice_roll, m);
    if (m->x1 == -1)
      continue;
    char player = board_pop(b, m->x1, m->y1);
    board_push(b, m->x2, m->y2, player);
    if (m->y2 == b->n_rows - 1) {
      b->n_finished[(int)(player - 'A')] += 1;
      if (b->n_finished[(int)(player - 'A')] == min(3, N_HEDGE - 2)) {
        winning_player = (int)(player - 'A');
        b->game_is_finished = true;
      }
    }
  }
  if (b->game_is_finished) {
    if (b->current_player == N_PLAYERS - 1) {
      free(m);
      return winning_player;
    }
  }
  b->current_player += 1;
  b->current_player %= N_PLAYERS;
  free(m);
  return -1;
}

// test if the player has a play
bool test_possible_move(board_t *b, step s, int dice_roll) {
  bool possible_move = false;
  if (s == VERTICAL) {
    for (int i = 0; i < b->n_lines; i += 1) {
      for (int j = 0; j < b->n_rows - 1; j += 1) {
        if (board_top(b, i, j) == b->current_player + 'A') {
          if (!b->is_trapped[i][j]) {
            possible_move = true;
            break;
          }
        }
      }
    }
    if (!possible_move) {
      ui_printf("Sorry, no possible move for you here.\n");
      ui_show(b); // let the user acknowledge
      return possible_move;
    }
  }
  if (s == HORIZONTAL) {
    for (int j = 0; j < b->n_rows - 1; j += 1) {
      if (board_height(b, dice_roll - 1, j) != 0) {
        possible_move = true;
        break;
      }
    }
    if (!possible_move) {
      ui_printf("Sorry, no possible move for you here.\n");
      ui_show(b);
      return possible_move;
    }
  }
  return possible_move;
}

// only reads the inputs, does not change the board ("getter")
void scan_move(board_t *b, step s, int dice_roll, move* m) {
  ui_render_board(b, dice_roll - 1);
  bool is_correct = false;
  ui_printf("Player %c is currently playing. \n", b->current_player + 'A');
  ui_printf("Randomly selected line for the horizontal play: %d.\n", dice_roll);
  while (!is_correct) {
    bool possible_move = test_possible_move(b, s, dice_roll);
    if (!possible_move) {
      m->x1 = -1;
      return;
    }
    ui_printf("[y%s] of the %s moving hedgehog.\n", s == VERTICAL ? ", x" : "",
              s == VERTICAL ? "vertically" : "horizontally");
    if (s == VERTICAL)
      ui_printf("Enter [skip] if you want to skip.\n");
    unsigned int x_atoi[1];
    unsigned int y_atoi[1];
    int i2c = am_input_2_coordinates(x_atoi, y_atoi, s, dice_roll);
    while (i2c == 0) {
      i2c = am_input_2_coordinates(x_atoi, y_atoi, s, dice_roll);
    }
    if (i2c == -1) {
      m->x1 = -1;
      return;
    }
    bool input_is_correct = test_input_is_correct(b, x_atoi[0], y_atoi[0]);
    if (!input_is_correct) {
      continue;
    }
    x_atoi[0] -= 1;
    y_atoi[0] -= 1;
    m->x1 = x_atoi[0];
    m->y1 = y_atoi[0];
    if (board_height(b, x_atoi[0], y_atoi[0]) == 0) {
      ui_printf(
          "It seems that you do not have a hedgehog to move in this cell.\n");
      continue;
    }
    if (b->is_trapped[x_atoi[0]][y_atoi[0]]) {
      bool found_hedgehog = false;
      for (int j = 0; (unsigned int)j < y_atoi[0]; j += 1) {
        if (board_height(b, x_atoi[0], j) > 0) {
          found_hedgehog = true;
          break;
        }
      }
      if (found_hedgehog) {
        ui_printf("You are trying to move a trapped hedgehog. All the cells "
                  "before the hedgehog must be emptied to enable the move.\n");
        continue;
      }
    }
    if (y_atoi[0] == (unsigned int)b->n_rows - 1) {
      ui_printf("The hedgehog has already finished playing.\n");
      continue;
    }
    switch (s) {
    case (HORIZONTAL):
      am_assign_move_horizontal(b, m, x_atoi, y_atoi, s, dice_roll);
      break;
    case (VERTICAL):
      am_assign_move_vertical(b, m, x_atoi, y_atoi, s, dice_roll);
      break;
    }

    is_correct = true;
  }
  return;
}
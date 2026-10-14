#include "all_move.h"

am_mode a_mode;

void am_mode_init(am_mode m) { a_mode = m; }

int max(int a, int b) { return (a > b ? a : b); }

// transform the input in coordinates
int am_input_2_coordinates(unsigned int *x_atoi, unsigned int *y_atoi, step s,
                           int dice_roll) {
  // 0: error, -1: skip, 1: ok
  char input[50];
  fflush(stdin);
  ui_scanf(" %49[^\n]", input);
  if (strcmp(input, "skip") == 0) {
    if (s == HORIZONTAL) {
      ui_printf("Sorry, you cannot skip.\n");
      return 0;
    } else {
      ui_printf("You skipped.\n");
      return -1;
    }
  }
  int ind_input = 0;
  int ind_xy = 0;
  char x_input[5] = {'\0'};
  char y_input[5] = {'\0'};
  bool is_sep = false;
  while (input[ind_input] != '\0') {
    if ('0' <= input[ind_input] && input[ind_input] <= '9') {
      if (is_sep && (s == VERTICAL || a_mode == ALL_MOVE)) {
        x_input[ind_xy] = input[ind_input];
      } else {
        y_input[ind_xy] = input[ind_input];
      }
      ind_xy += 1;
    } else {
      if (ind_xy > 0) {
        if (s == HORIZONTAL && a_mode == CLASSIC)
          break;
        if (is_sep == true)
          return 1;

        is_sep = true;
        ind_xy = 0;
      }
    }
    ind_input += 1;
  }
  x_atoi[0] = (unsigned int)atoi(x_input);
  y_atoi[0] = (unsigned int)atoi(y_input);
  if (s == HORIZONTAL && dice_roll != -1) {
    x_atoi[0] = (unsigned int)dice_roll;
  }
  return 1;
}

// test if the coordinates point to a correct cell
bool test_input_is_correct(board_t *b, unsigned int x_atoi,
                           unsigned int y_atoi) {
  bool input_is_correct = true;
  if (y_atoi == 0 || (x_atoi == 0)) {
    ui_printf("It seems that your input is not a number or that you have "
              "forgotten a coordinate.\n");
    input_is_correct = false;
    return input_is_correct;
  }
  if (x_atoi > (unsigned int)b->n_lines || y_atoi > (unsigned int)b->n_rows) {
    ui_printf("It seems that your input is outside the board.\n");
    input_is_correct = false;
    return input_is_correct;
  }
  return input_is_correct;
}

int valabs(int a) { return (a < 0 ? (-a) : a); }

// test if the move is diagonal, vertical, or horizontal
int test_move_is_ok(move *m) {
  // 0: error, 1: vertical, 2: horizontal, 3: diagonal
  int cell_is_correct = 0;
  if (m->y1 == m->y2) {
    cell_is_correct = 1;
  }
  if (m->x1 == m->x2) {
    cell_is_correct = 2;
  }
  if (!cell_is_correct) {
    if (valabs(m->x1 - m->x2) == valabs(m->y1 - m->y2)) {
      cell_is_correct = 3;
    }
  }
  return cell_is_correct;
}
void am_assign_move_horizontal(board_t *b, move *m, unsigned int *x_atoi,
                               unsigned int *y_atoi, step s) {
  if (a_mode == CLASSIC) {
    m->x2 = x_atoi[0];
    m->y2 = y_atoi[0] + 1;
  } else {
    bool move_is_correct = false;
    while (!move_is_correct) {
      ui_printf("[y, x] of the destination.\n");
      int i2c = am_input_2_coordinates(x_atoi, y_atoi, s, -1);
      while (i2c == 0) {
        i2c = am_input_2_coordinates(x_atoi, y_atoi, s, -1);
      }
      bool input_is_correct = test_input_is_correct(b, x_atoi[0], y_atoi[0]);
      if (!input_is_correct) {
        continue;
      }
      x_atoi[0] -= 1;
      y_atoi[0] -= 1;
      m->x2 = x_atoi[0];
      m->y2 = y_atoi[0];
      int move_is_ok = test_move_is_ok(m);
      int min_x = min(m->x1, m->x2);
      int max_x = max(m->x1, m->x2);
      int min_y = min(m->y1, m->y2);
      int max_y = max(m->y1, m->y2);
      switch (move_is_ok) {
      case 1:
        for (int i = min_x; i < max_x; i += 1) {
          if (b->is_trapped[i][m->y1]) {
            m->x2 = i;
            break;
          }
        }
        move_is_correct = true;
        break;
      case 2:
        for (int j = min_y; j < max_y; j += 1) {
          if (b->is_trapped[m->x1][j]) {
            m->y2 = j;
            break;
          }
        }
        move_is_correct = true;
        break;
      case 3: {
        int dx = (m->x2 > m->x1) ? 1 : -1;
        int dy = (m->y2 > m->y1) ? 1 : -1;
        for (int i = m->x1 + dx, j = m->y1 + dy;
             i != m->x2 + dx && j != m->y2 + dy; i += dx, j += dy) {
          if (b->is_trapped[i][j]) {
            m->x2 = i;
            m->y2 = j;
            break;
          }
        }
      }
        move_is_correct = true;
        break;
      default:
        ui_printf("You cannot move the hedgehog here.\n");
        break;
      }
    }
  }
  return;
}

int am_assign_move_vertical(board_t *b, move *m, unsigned int *x_atoi,
                            unsigned int *y_atoi, step s, int dice_roll) {
  if (a_mode == CLASSIC) {
    m->y2 = y_atoi[0];
    if (board_top(b, x_atoi[0], y_atoi[0]) != b->current_player + 'A') {
      m->x1 = -1;
      ui_printf("You are trying to move the hedgehog of another player.\n");
      return -1;
    }
    char side;
    ui_printf("Choose a direction to move the hedgehog: [u] for up or [d] "
              "for down.\n");
    ui_scanf(" %c", &side);
    switch (side) {
    case ('H'):
    case ('h'):
    case ('T'):
    case ('t'):
    case ('U'):
    case ('u'):
      if (x_atoi[0] == 0) {
        ui_printf("You cannot move higher.\n");
        return -1;
      }
      m->x2 = x_atoi[0] - 1;
      break;
    case ('B'):
    case ('b'):
    case ('D'):
    case ('d'):
      if (x_atoi[0] == (unsigned int)b->n_lines - 1) {
        ui_printf("You cannot move lower.\n");
        return -1;
      }
      m->x2 = x_atoi[0] + 1;
      break;
    default:
      ui_printf("It seems that you have made a mistake.\n");
      return -1;
    }
  } else {
    bool move_is_correct = false;
    while (!move_is_correct) {
      ui_printf("[y, x] of the destination.\n");
      int i2c = am_input_2_coordinates(x_atoi, y_atoi, s, dice_roll);
      while (i2c == 0) {
        i2c = am_input_2_coordinates(x_atoi, y_atoi, s, dice_roll);
      }
      bool input_is_correct = test_input_is_correct(b, x_atoi[0], y_atoi[0]);
      if (!input_is_correct) {
        continue;
      }
      x_atoi[0] -= 1;
      y_atoi[0] -= 1;
      m->x2 = x_atoi[0];
      m->y2 = y_atoi[0];
      int move_is_ok = test_move_is_ok(m);
      int min_x = min(m->x1, m->x2);
      int max_x = max(m->x1, m->x2);
      int min_y = min(m->y1, m->y2);
      int max_y = max(m->y1, m->y2);
      switch (move_is_ok) {
      case 1:
        for (int i = min_x; i < max_x; i += 1) {
          if (b->is_trapped[i][m->y1]) {
            m->x2 = i;
            break;
          }
        }
        move_is_correct = true;
        break;
      case 2:
        for (int j = min_y; j < max_y; j += 1) {
          if (b->is_trapped[m->x1][j]) {
            m->y2 = j;
            break;
          }
        }
        move_is_correct = true;
        break;
      case 3: {
        int dx = (m->x2 > m->x1) ? 1 : -1;
        int dy = (m->y2 > m->y1) ? 1 : -1;
        for (int i = m->x1 + dx, j = m->y1 + dy;
             i != m->x2 + dx && j != m->y2 + dy; i += dx, j += dy) {
          if (b->is_trapped[i][j]) {
            m->x2 = i;
            m->y2 = j;
            break;
          }
        }
      }
        move_is_correct = true;
        break;
      default:
        ui_printf("You cannot move the hedgehog here.\n");
        break;
      }
    }
  }
  return 0;
}

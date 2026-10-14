#include "all_move.h"

am_mode a_mode ;

void am_mode_init(am_mode m){
  a_mode = m;
}

int i2c_horizontal(unsigned int* x_atoi, unsigned int* y_atoi){
    char input[50];
    fflush(stdin);
    ui_scanf(" %49[^\n]", input);
    if (strcmp(input, "skip") == 0) {
        ui_printf("Sorry, you cannot skip.\n");
        return 0;
    }
    int ind_input = 0;
    int ind_xy = 0;
    char x_input[5] = {'\0'};
    char y_input[5] = {'\0'};
    bool is_sep = false;
    while (input[ind_input] != '\0') {
      if ('0' <= input[ind_input] && input[ind_input] <= '9') {
        if (is_sep) {
          x_input[ind_xy] = input[ind_input];
        } else {
          y_input[ind_xy] = input[ind_input];
        }
        ind_xy += 1;
      } else {
        if (ind_xy > 0) {
          if (is_sep == true)
            return 1;

          is_sep = true;
          ind_xy = 0;
        }
      }
      ind_input += 1;
    }
    x_atoi[0] = (unsigned int) atoi(x_input);
    y_atoi[0] = (unsigned int) atoi(y_input);
    return 1;
}

bool test_input_is_correct(board_t* b, unsigned int x_atoi, unsigned int y_atoi){
  bool input_is_correct = true ;
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

int valabs(int a){
    return (a < 0 ? (-a) : a) ;
}

bool test_new_cell_is_ok(move* m){
    bool cell_is_correct = (m->x1 == m->x2 || m->y1 == m->y2) ;
    if(!cell_is_correct){
        if(valabs(m->x1 - m->x2) == valabs(m->y1 - m->y2)){
            cell_is_correct = true ;
        }
    }
    return cell_is_correct ;
}

void am_horizontal(board_t* b, move *m, unsigned int* x_atoi, unsigned int* y_atoi){
  if(a_mode == CLASSIC){
    m->x2 = x_atoi[0];
    m->y2 = y_atoi[0] + 1;
  }
  else{
    ui_printf("Coucou je suis pas cense etre la.\n");
    bool move_is_correct = false ;
    while(!move_is_correct){
    ui_printf("[y, x] of the moving hedgehog.\n");
    int i2c = i2c_horizontal(x_atoi, y_atoi);
    while(i2c ==0){
      i2c = i2c_horizontal(x_atoi, y_atoi);
    }
    bool input_is_correct = test_input_is_correct(b, x_atoi[0], y_atoi[0]);
    if(!input_is_correct){
      continue;
    }
    x_atoi[0] -= 1;
    y_atoi[0] -= 1;
    m->x2 = x_atoi[0];
    m->y2 = y_atoi[0];
    bool new_cell_is_ok = test_new_cell_is_ok(m);
    if(new_cell_is_ok){
      move_is_correct = true;
    }
    else{
      ui_printf("You cannot move the hedgehog here.\n");
    }
    }
  }
  return;
}

void am_vertical(board_t* b,move*m, unsigned int* x_atoi, unsigned int* y_atoi){
  if(a_mode == CLASSIC){
      m->y2 = y_atoi[0];
      if (board_top(b, x_atoi[0], y_atoi[0]) != b->current_player + 'A') {
        ui_printf("You are trying to move the hedgehog of another player.\n");
        return;
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
          return;
        }
        m->x2 = x_atoi[0] - 1;
        break;
      case ('B'):
      case ('b'):
      case ('D'):
      case ('d'):
        if (x_atoi[0] == (unsigned int)b->n_lines - 1) {
          ui_printf("You cannot move lower.\n");
          return;
        }
        m->x2 = x_atoi[0] + 1;
        break;
      default:
        ui_printf("It seems that you have made a mistake.\n");
        return;
      }
    }
    else{
    ui_printf("Coucou je suis pas cense etre la.\n");
    bool move_is_correct = false ;
    while(!move_is_correct){
    ui_printf("[y, x] of the moving hedgehog.\n");
    int i2c = i2c_horizontal(x_atoi, y_atoi);
    while(i2c ==0){
      i2c = i2c_horizontal(x_atoi, y_atoi);
    }
    bool input_is_correct = test_input_is_correct(b, x_atoi[0], y_atoi[0]);
    if(!input_is_correct){
      continue;
    }
    x_atoi[0] -= 1;
    y_atoi[0] -= 1;
    m->x2 = x_atoi[0];
    m->y2 = y_atoi[0];
    bool new_cell_is_ok = test_new_cell_is_ok(m);
    if(new_cell_is_ok){
      move_is_correct = true;
    }
    else{
      ui_printf("You cannot move the hedgehog here.\n");
    }
    }
    }
}


/*void input_to_coordinate(char* input, char* x_input, char* y_input, step s){
    int ind_input = 0;
    int ind_xy = 0;
    bool is_sep = false;
    while (input[ind_input] != '\0') {
      if ('0' <= input[ind_input] && input[ind_input] <= '9') {
        if (is_sep && s == VERTICAL) {
          x_input[ind_xy] = input[ind_input];
        } else {
          y_input[ind_xy] = input[ind_input];
        }
        ind_xy += 1;
      } else {
        if (ind_xy > 0) {
          if (s == HORIZONTAL){
            break;
          }
          if (is_sep == true){
            break;
          }
          is_sep = true;
          ind_xy = 0;
        }
      }
      ind_input += 1;
    }
}

void test_input_is_correct(board_t* b, unsigned int x_atoi, unsigned int y_atoi){
    if (y_atoi == 0 || (x_atoi == 0)) {
      ui_printf("It seems that your input is not a number or that you have "
                "forgotten a coordinate.\n");
    }
    if (x_atoi > (unsigned int)b->n_lines || y_atoi > (unsigned int)b->n_rows) {
      ui_printf("It seems that your input is outside the board.\n");
    }
}

bool test_new_cell_is_correct(move m){
    bool cell_is_correct = (m.x1 == m.x2 || m.y1 == m.y2) ;
    if(!cell_is_correct){
        if(valabs(m.x1 - m.x2) == valabs(m.y1 - m.y2)){
            cell_is_correct = true ;
        }
    }
    return cell_is_correct ;
}

bool test_possible_move(board_t* b, step s, int dice_roll){
    bool possible_move ;
    if (s == VERTICAL) {
      possible_move = false;
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
        ui_show();
        return possible_move;
      }
    }
    if (s == HORIZONTAL) {
      possible_move = false;
      for (int j = 0; j < b->n_rows - 1; j += 1) {
        if (board_height(b, dice_roll - 1, j) != 0) {
          possible_move = true;
          break;
        }
      }
      if (!possible_move) {
        ui_printf("Sorry, no possible move for you here.\n");
        ui_show();
        return possible_move ;
      }
    }
    return possible_move;
}

bool test_move_is_okay(board_t* b, int x_atoi, int y_atoi){
    bool move_is_okay = true ;
    if (board_height(b, x_atoi, y_atoi) == 0) {
      ui_printf(
          "It seems that you do not have a hedgehog to move in this cell.\n");
          move_is_okay = false ;
          return move_is_okay ;
    }
    if (b->is_trapped[x_atoi][y_atoi]) {
      for (int j = 0; (unsigned int)j < y_atoi; j += 1) {
        if (board_height(b, x_atoi, j) > 0) {
          move_is_okay = false ;
          break;
        }
      }
      if (!move_is_okay) {
        ui_printf("You are trying to move a trapped hedgehog. All the cells "
                  "before the hedgehog must be emptied to enable the move.\n");
      }
    }
    return move_is_okay;
}


// only reads the inputs, does not change the board ("getter")
move am_scan_move(board_t *b, step s, int dice_roll) {
  ui_render_board(b, dice_roll - 1);
  move m;
  bool is_correct = false;
  ui_printf("Player %c is currently playing. \n", b->current_player + 'A');
  ui_printf("Randomly selected line for the horizontal play: %d.\n", dice_roll);
  unsigned int x_atoi ;
  unsigned int y_atoi ;
    char input[50];
    char x_input[5] = {'\0'};
    char y_input[5] = {'\0'};
  while (!is_correct) {
    bool possible_move = test_possible_move(b, s, dice_roll) ;
    if(!possible_move){
        m.x1 = -1 ;
        return m ;
    }
    ui_printf("[y%s] of the %s moving hedgehog.\n", s == VERTICAL ? ", x" : "",
              s == VERTICAL ? "vertically" : "horizontally");
    if (s == VERTICAL){
        ui_printf("Enter [skip] if you want to skip.\n");
    }
    fflush(stdin);
    ui_scanf("%49[^\n]", input);
    if (strcmp(input, "skip") == 0) {
      if (s == HORIZONTAL) {
        ui_printf("Sorry, you cannot skip.\n");
        continue;
      } else {
        ui_printf("You skipped.\n");
        m.x1 = -1;
        return m;
      }
    }
    input_to_coordinate(input, x_input, y_input, s) ;
    x_atoi = s == HORIZONTAL ? dice_roll : atoi(x_input);
    y_atoi = atoi(y_input);
    test_input_is_correct(b, x_atoi, y_atoi) ;
    x_atoi -= 1;
    y_atoi -= 1;
    m.x1 = x_atoi;
    m.y1 = y_atoi;
    bool move_is_okay = test_move_is_okay(b, x_atoi, y_atoi) ;
    if(!move_is_okay){
        continue;
    }
    else{
        is_correct = true;
    }
  }
  is_correct = false ;
  while(!is_correct){
    bool new_cell_is_correct ;
    switch (s) {
    case (HORIZONTAL):
      if (y_atoi == (unsigned int)b->n_rows - 1) {
        ui_printf("The hedgehog has already finished playing.\n");
        continue;
      }
      ui_printf("[x, y] of the new cell to move the hedgehog.\n");
      fflush(stdin);
      input_to_coordinate(input, x_input, y_input, s) ;
      x_atoi = dice_roll;
      y_atoi = atoi(y_input);
      test_input_is_correct(b, x_atoi, y_atoi) ;
      x_atoi -= 1;
      y_atoi -= 1;
      new_cell_is_correct = test_new_cell_is_correct(m) ;
      if(!new_cell_is_correct){
        ui_printf("This move is incorrect. You can only play horizontally or diagonally.\n") ;
        continue ;
      }
      else{
        m.x2 = x_atoi;
        m.y2 = y_atoi;
      }
      break;
    case (VERTICAL):
      m.y2 = y_atoi;
      if (board_top(b, x_atoi, y_atoi) != b->current_player + 'A') {
        ui_printf("You are trying to move the hedgehog of another player.\n");
        continue;
      }
      ui_printf("[x, y] of the new cell to move the hedgehog.\n");
      fflush(stdin);
      input_to_coordinate(input, x_input, y_input, s) ;
      x_atoi = atoi(x_input);
      y_atoi = atoi(y_input);
      test_input_is_correct(b, x_atoi, y_atoi) ;
      x_atoi -= 1;
      y_atoi -= 1;
      new_cell_is_correct = test_new_cell_is_correct(m) ;
      if(!new_cell_is_correct){
        ui_printf("This move is incorrect. You can only play vertically or diagonally.\n") ;
        continue ;
      }
      else{
        m.x2 = x_atoi;
        m.y2 = y_atoi;
      }
      break;
    }
    is_correct = true;
  }
  return m;
}*/

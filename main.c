#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "all_move.h"
#include "primitives.h"
#include "string.h"
#include "tests.h"
#include "ui.h"
#include <time.h>

int main(int argc, char **argv) {

  srand(time(NULL));

  // check user input
  if (GAME_SIZE_X <= 0 || GAME_SIZE_Y <= 0) {
    printf("[ERROR] Wrong game dimensions input !\n");
    return -1;
  }

  if (N_PLAYERS <= 0) {
    printf("[ERROR] Wrong player count !");
    return -1;
  }
  if (N_PLAYERS > 4) {
    printf("[WARNING] This number of players will throw an error in GUI mode, "
           "which wasn't designed to play with more than 4 players.\n");
  }
  if (N_HEDGE <= 0) {
    printf("[ERROR] Wrong hedge count !\n");
    return -1;
  }
  if (SCREEN_WIDTH <= 0 || SCREEN_HEIGHT <= 0 || CELL_SIZE <= 0) {
    printf("[ERROR] Wrong GUI dimensions\n");
    return -1;
  }


  // check whether modes are on
  bool is_gui = false;
  bool is_am = false;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--gui") == 0 || strcmp(argv[i], "-g") == 0) {
      is_gui = true;
    }
    if (strcmp(argv[i], "-am") == 0 || strcmp(argv[i], "--all-move") == 0) {
      is_am = true;
    }
  }

  if (is_gui) {
#ifdef ENABLE_GUI
    ui_init(UI_MODE_GRAPHIC);
#else
    printf("[ERROR] To use GUI mode, you need to compile with `make gui` and "
           "install dependencies\n");
    exit(1);
#endif
  } else {
    ui_init(UI_MODE_TERM);
  }

  if (is_am) {
    am_mode_init(ALL_MOVE);
  } else {
    am_mode_init(CLASSIC);
  }


  // beginning of the game
  board_t boa = board_init();

  int winning_player;
  while ((winning_player = play_turn(&boa)) == -1) {
  }
  ui_render_board(&boa, 0);
  ui_printf("Game is finished! Thank you for playing.\n");

  int **score_team = malloc(sizeof(int *) * N_HEDGE);
  for (int i = 0; i < N_HEDGE; i += 1) {
    score_team[i] = malloc(sizeof(int) * (N_PLAYERS + 1));
    for (int j = 1; j < N_PLAYERS + 1; j += 1) {
      score_team[i][j] = -1;
    }
    score_team[i][0] = 0;
  }
  // i: number of hedgehogs // j: teams with i finalist hedgehogs
  // score_team[i][0]: length of score_team[i], score_team[i][j]: teams id
  for (int i = 0; i < N_PLAYERS; i += 1) {
    int nb_hedge = boa.n_finished[i];
    int indice = 1;
    assert(nb_hedge < N_HEDGE);
    // should never fail
    while (indice < N_PLAYERS + 1 && score_team[nb_hedge][indice] != -1) {
      indice += 1;
    }
    assert(indice < N_PLAYERS + 1);
    // should never fail either
    score_team[nb_hedge][indice] = i;
    score_team[nb_hedge][0] += 1;
  }

  // scoring
  int rank = 1;
  for (int i = N_HEDGE - 1; i >= 0; i -= 1) {
    if (score_team[i][0] != 0) {
      ui_printf("Rank #%d: ", rank);
      for (int j = 0; j < score_team[i][0]; j += 1) {
        ui_printf("team %c, ", score_team[i][j + 1] + 'A');
      }
      ui_printf("\b\b (with %d hedgehog%s)\n", i, i > 1 ? "s" : "");
      rank += 1;
    }
  }

  for (int i = 0; i < N_HEDGE; i += 1) {
    free(score_team[i]);
  }

  ui_show();

  free(score_team);

  board_free(boa);

  ui_free();

  return 0;
}
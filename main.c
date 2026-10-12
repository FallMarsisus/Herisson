#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "primitives.h"
#include "string.h"
#include "tests.h"
#include "ui.h"
#include <time.h>

int main(int argc, char **argv) {

  srand(time(NULL));

  // Check user input
  if (GAME_SIZE_X <= 0 || GAME_SIZE_Y <= 0) {
    printf("[ERROR] Wrong game dimensions input !\n");
    return -1;
  }

  if (N_PLAYERS <= 0) {
    printf("[ERROR] Wrong player count !");
    return -1;
  }
  if (N_PLAYERS < 4) {
    printf("[WARNING] This number of players will throw an error in GUI mode, "
           "which wasn't designed to play with > 4 players.\n");
  }

  if (N_HEDGE <= 0) {
    printf("[ERROR] Wrong hedge count !\n");
    return -1;
  }

  if (SCREEN_WIDTH <= 0 || SCREEN_HEIGHT <= 0 || CELL_SIZE <= 0) {
    printf("[ERROR] Wrong gui dimensions\n");
    return -1;
  }

  if (argc > 1 &&
      (strcmp(argv[1], "--gui") == 0 || strcmp(argv[1], "-g") == 0)) {
#ifdef ENABLE_GUI
    ui_init(UI_MODE_GRAPHIC);
#else
      printf("[ERROR] To use GUI mode, you need to compile with `make gui` and install dependencies\n");
      exit(1);
  } else {
    ui_init(UI_MODE_TERM);
  }

   board_t boa = board_init();

  int winning_player;
  while ((winning_player = play_turn(&boa)) == -1) {
  }
  ui_render_board(&boa, 0);
  ui_printf("Congratulations! Player %c has won the game! ",
            winning_player + 'A');
  ui_printf("Thank you for playing!\n");

  int **trier_team = malloc(sizeof(int *) * N_HEDGE);
  for (int i = 0; i < N_HEDGE; i += 1) {
    trier_team[i] = malloc(sizeof(int) * N_PLAYERS);
    for (int j = 0; j < N_PLAYERS; j += 1) {
      trier_team[i][j] = -1;
    }
  }
  // i : number of hedgehogs // j : team
  for (int i = 0; i < N_PLAYERS; i += 1) {
    int nb_hedge = boa.n_finished[i];
    int indice = 0;
    while (indice < N_PLAYERS && trier_team[nb_hedge][indice] != -1) {
      indice += 1;
    }
    if (indice >= N_PLAYERS) {
      ui_printf("[ERROR] Wrong count when counting final result.\n");
      exit(1);
    }
    trier_team[nb_hedge][indice] = i;
  }

  int rank = 1;
  for (int i = N_HEDGE - 1; i >= 0; i -= 1) {
    bool there_is_a_player = false;
    for (int j = 0; j < N_PLAYERS; j += 1) {
      if (trier_team[i][j] != -1) {
        ui_printf("Rank #%d: team %c with %d hedgehog%s.\n", rank,
                  trier_team[i][j] + 'A', i, i > 1 ? "s" : "");
        there_is_a_player = true;
      }
    }
    if (there_is_a_player) {
      rank += 1;
    }
  }

  for (int i = 0; i < N_HEDGE; i += 1) {
    free(trier_team[i]);
  }

  free(trier_team);

  ui_show();

  board_free(boa);

  ui_free();

  return 0;
}
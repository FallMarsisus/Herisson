#include "tests.h"

void test_board(void)
{
	board_t boa = board_init();

	board_push(&boa, 3, 4, 'B');
	board_push(&boa, 3, 4, 'A');
	board_push(&boa, 3, 4, 'C');
	board_push(&boa, 3, 4, 'A');

	ui_render_board(&boa, 0);

	board_free(boa);
}

void test_play(void){
	board_t boa = board_init();

	int winning_player ;
	while((winning_player=play_turn(&boa)) == -1){
	}
	ui_render_board(&boa, 0) ;
	ui_printf("Congratulations! Player %c has won the game! ", winning_player + 'A') ;
	ui_printf("Thank you for playing!\n") ;

	board_free(boa);
}

void test_score(void){
	board_t boa = board_init();

	int winning_player ;
	while((winning_player=play_turn(&boa)) == -1){
	}
	ui_render_board(&boa, 0) ;
	ui_printf("Game is finished! Thank you for playing.\n") ;

	int** score_team = malloc(sizeof(int*)*N_HEDGE) ;
	for(int i = 0 ; i < N_HEDGE ; i += 1){
		score_team[i] = malloc(sizeof(int)*(N_PLAYERS+1)) ;
		for(int j = 1 ; j < N_PLAYERS+1 ; j += 1){
			score_team[i][j] = -1 ;
		}
		score_team[i][0] = 0 ;
	}
	// i: number of hedgehogs // j: teams with i finalist hedgehogs
	// score_team[i][0]: length of score_team[i], score_team[i][j]: teams id
	for(int i = 0 ; i < N_PLAYERS ; i += 1){
		int nb_hedge = boa.n_finished[i] ;
		int indice = 1 ;
		assert(nb_hedge < N_HEDGE) ;
		// should never fail
		while(indice < N_PLAYERS+1 && score_team[nb_hedge][indice] != -1){ 
			indice += 1 ;
		}
		assert(indice < N_PLAYERS+1) ;
		// should never fail too
		score_team[nb_hedge][indice] = i ;
		score_team[nb_hedge][0] += 1 ;
	}

	int rank = 1 ;
	for(int i = N_HEDGE-1 ; i >= 0 ; i -= 1){
		if(score_team[i][0] != 0){
			ui_printf("Rank #%d: ", rank) ;
			for(int j = 0 ; j < score_team[i][0] ; j += 1){
				ui_printf("team %c, ", score_team[i][j+1]+'A') ;
			}
			ui_printf("\b\b (with %d hedgehog%s)\n", i, i > 1 ? "s" : "") ;
			rank += 1 ;
		}
	}

	for(int i = 0 ; i < N_HEDGE ; i += 1){
		free(score_team[i]) ;
	}
	free(score_team) ;

	board_free(boa);
}

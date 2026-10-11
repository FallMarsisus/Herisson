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
	ui_printf("Congratulations! Player %c has won the game! ", winning_player + 'A') ;
	ui_printf("Thank you for playing!\n") ;

	int** trier_team = malloc(sizeof(int*)*N_HEDGE) ;
	for(int i = 0 ; i < N_HEDGE ; i += 1){
		trier_team[i] = malloc(sizeof(int)*N_PLAYERS) ;
		for(int j = 0 ; j < N_PLAYERS ; j += 1){
			trier_team[i][j] = -1 ;
		}
	}
	// i : number of hedgehogs // j : team
	for(int i = 0 ; i < N_PLAYERS ; i += 1){
		int nb_hedge = boa.n_finished[i] ;
		int indice = 0 ;
		while(indice < N_PLAYERS && trier_team[nb_hedge][indice] != -1){ 
			indice += 1 ;
		}
		if(indice >= N_PLAYERS){
			ui_printf("GROS GROS GROS BUG WTF\n") ;
			exit(1) ;
		}
		trier_team[nb_hedge][indice] = i ;
	}

	int rank = 1 ;
	for(int i = N_HEDGE-1 ; i >= 0 ; i -= 1){
		bool there_is_a_player = false ;
		for(int j = 0 ; j < N_PLAYERS ; j += 1){
			if(trier_team[i][j] != -1){
				ui_printf("Rank #%d: team %c with %d hedgehog%s.\n", rank, trier_team[i][j]+'A', i, i > 1 ? "s" : "") ;
				there_is_a_player = true ;
			}
		}
		if(there_is_a_player){
			rank += 1 ;
		}
	}

	for(int i = 0 ; i < N_HEDGE ; i += 1){
		free(trier_team[i]) ;
	}

	free(trier_team) ;

	ui_show();

	board_free(boa);
}
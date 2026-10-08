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

	ui_render_board(&boa, 0);

	int winning_player = 0 ;
	while((winning_player=play_turn(&boa)) == -1){
		ui_render_board(&boa, 0);
	}
	printf("Congratulations! Player %d has won the game!\n", winning_player) ;
	printf("Thank you for playing!\n") ;

	board_free(boa);
}
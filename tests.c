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

	int winning_player = 0 ;
	while((winning_player=play_turn(&boa)) == -1){
	}
	ui_render_board(&boa, 0) ;
	ui_printf("Congratulations! Player %c has won the game! ", winning_player + 'A') ;
	ui_printf("Thank you for playing!\n") ;

	board_free(boa);
}
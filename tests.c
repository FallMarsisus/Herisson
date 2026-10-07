#include "tests.h"

void test_board()
{
	board_t boa = board_init();

	board_push(&boa, 3, 4, 'B');
	board_push(&boa, 3, 4, 'A');
	board_push(&boa, 3, 4, 'C');
	board_push(&boa, 3, 4, 'A');

	board_print(&boa, 0);

	board_free(boa);
}

void test_play(){
	board_t boa = board_init();

	board_push(&boa, 3, 3, 'B');
	board_push(&boa, 2, 5, 'A');
	board_push(&boa, 0, 3, 'C');
	board_push(&boa, 0, 2, 'A');

	board_print(&boa, 0);

	while(play_turn(&boa) !=1){
		board_print(&boa, 0);
	}

	board_free(boa);
}
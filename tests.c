#include "tests.h"


void test_board() {
	board_t boa = board_init(); 

	board_print(&boa, 0);
}
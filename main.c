#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


#include "ui.h"
#include "tests.h"
#include <time.h>
#include "string.h"




int main(int argc, char** argv){

	srand(time(NULL));

	if (argc >1 && (strcmp(argv[1], "--gui") == 0 || strcmp(argv[1], "-g") == 0)) {
		ui_init(UI_MODE_GRAPHIC);
	} else {
		ui_init(UI_MODE_TERM);
	}
	
	test_play();

	
	ui_free();

	
	return 0; 
}
#include "board.h"
  
board_t board_init() {
	board_t b;
	b.n_lines = GAME_SIZE_Y; 
	b.n_rows = GAME_SIZE_X;
	b.board = malloc(sizeof(char**)*b.n_lines);
	for(int i = 0; i < b.n_lines; i++) {
		b.board[i] = malloc(sizeof(char*)*b.n_rows);
		for (int j = 0; j< b.n_rows; j++) {
			b.board[i][j] = malloc(sizeof(char)*N_PLAYERS);
		}
	}

	b.n_hedge = malloc(sizeof(int*)* b.n_lines);
	b.is_trapped = malloc(sizeof(bool*) * b.n_lines);
	for(int i = 0; i < b.n_lines; i++) {
		b.n_hedge[i] = malloc(sizeof(int)* b.n_rows);
		b.is_trapped[i] = malloc(sizeof(bool)*b.n_rows);
		for(int j = 0 ; j < b.n_rows; j++) {
			b.n_hedge[i][j] = 0; 
			b.is_trapped[i][j] = false;
		}
 	}

 	return b; 
}


void board_push(board_t *b, int line, int row, char ctn) {
  assert(line >= 0 && line < b->n_lines);
  assert(row >= 0 && row < b->n_rows);

  int i = b->n_hedge[line][row]; // position of the topmost element

  b->board[line][row][i] = ctn;

  b->n_hedge[line][row]++; // just added an element;
}

char board_pop(board_t *b, int line, int row) {
  assert(line >= 0 && line < b->n_lines);
  assert(row >= 0 && row < b->n_rows);

  int i = b->n_hedge[line][row];

  if (i >= 1) {
    b->n_hedge[line][row]--;
    return b->board[line][row][i];
  }

  return ' ';
}

// returns the number of hedgehogs in a specified place.
int board_height(board_t *b, int line, int row) {
  assert(line >= 0 && line < b->n_lines);
  assert(row >= 0 && row < b->n_rows);

  return b->n_hedge[line][row];
}

char board_top(board_t *b, int line, int row) {
  assert(line >= 0 && line < b->n_lines);
  assert(row >= 0 && row < b->n_rows);

  int i = b->n_hedge[line][row];

  return b->board[line][row][i];

}

char board_peek(board_t* b, int line, int row, int pos) {
	assert(line >= 0 && line < b->n_lines);
    assert(row >= 0 && row < b->n_rows);

    int i = b->n_hedge[line][row];

    if( pos < i ) {

    	return b->board[line][row][pos];

    }
    return ' '; 
}

void cell_print(board_t *b, int line, int row, int slice) {

	int n_hedge = b->n_hedge[line][row];
	bool is_trapped = b->is_trapped[line][row];

	char fst_line [4];
	char snd_line [4];
	char thd_line [4];
	
	switch (n_hedge){
		case 4:
			for(int i = 0; i < 3; i++) 
				snd_line[i] = b->board[line][row][i+1];
			break;
		case 3:
			snd_line[0] = b->board[line][row][1];
			snd_line[1] = ' ';
			snd_line[2] = b->board[line][row][2];
			break; 
		case 2: 
			for(int i = 0; i < 3; i++) 
				snd_line[i] = b->board[line][row][1];
			break;
		default:
			break;
	} 
	if(n_hedge >0) {
		for(int i = 0; i < 3; i++) 
			fst_line[i] = b->board[line][row][0];
	}
	else {
		for(int i = 0; i < 3; i++) {
			fst_line[i] = ' '; 
			snd_line[i] = ' '; 
	}
}	

	if(is_trapped) {
		for (int i = 0; i < 3; i++) 
			thd_line[i] = '^';
	}
	else {
		thd_line[0] = '-';
		thd_line[1] = n_hedge + '0';
		thd_line[2] = '-';
	}

	snd_line[3] = '\0';
	fst_line[3] = '\0';
	thd_line[3] = '\0';

	switch(slice) {

	case 0 :
		printf(" %s ", 
			is_trapped?"vvv":"---");
		break;
	case 1 : 
		printf("%c%s%c", 
			is_trapped?'>':'|',
			fst_line,
			is_trapped?'<':'|'); 
		break; 
	case 2 : 
		printf("%c%s%c",
			is_trapped?'>':'|',
			snd_line,
			is_trapped?'<':'|'); 
		break;
	case 3 : 
		printf(" %s ", 
			thd_line);
		break; 
	default:
		break; 

	}
}

void board_print(board_t* b, int highlighted_line) {
	// PERSO : pour l'instant n'affiche que les cases, 
	// affichera dans le futur le reste 

	for (int i = 0; i < b->n_lines*4; i++) {
		for(int j = 0; j< b->n_rows; j++) {
			cell_print(b, i/4, j, i%4);
		}
		printf("\n");
	}
}

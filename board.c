#include "board.h"


void allocate_trap(board_t* b, int line, int row){
	if(line < b->n_lines && line >= 0 && row < b->n_rows && row >= 0 ){
		b->is_trapped[line][row] = true ;
	}
}

board_t board_init(void)
{
	// creation+initialisation of board
	board_t b;
	b.n_lines = GAME_SIZE_Y;
	b.n_rows = GAME_SIZE_X;
	b.board = malloc(sizeof(char **) * b.n_lines);
	for (int i = 0; i < b.n_lines; i++)
	{
		b.board[i] = malloc(sizeof(char *) * b.n_rows);
		for (int j = 0; j < b.n_rows; j++)
		{
			b.board[i][j] = malloc(sizeof(char) * N_PLAYERS);
		}
	}

	// creation+initialisation of other tables for hedges, traps and end blocks
	b.n_hedge = malloc(sizeof(int *) * b.n_lines);
	b.is_trapped = malloc(sizeof(bool *) * b.n_lines);
	for (int i = 0; i < b.n_lines; i++)
	{
		b.n_hedge[i] = malloc(sizeof(int) * b.n_rows);
		b.is_trapped[i] = malloc(sizeof(bool) * b.n_rows);
		for (int j = 0; j < b.n_rows; j++)
		{
			b.n_hedge[i][j] = 0;
			b.is_trapped[i][j] = false;
		}
	}
	b.n_finished = malloc(sizeof(int) * N_PLAYERS);
	for (int i = 0; i < N_PLAYERS; i++)
	{
		b.n_finished[i] = 0;
	}

	// traps are places
	allocate_trap(&b, 0, 2);
	allocate_trap(&b, 1, 6);
	allocate_trap(&b, 2, 4); 
	allocate_trap(&b, 3, 5);
	allocate_trap(&b, 4, 3); 
	allocate_trap(&b, 5, 7);

	// hedges are placed randomly
	int placed_hedgehogs = 0 ;
	int* placed_hedgehog_player = malloc(sizeof(int)*N_PLAYERS) ;
	for(int i = 0 ; i < N_PLAYERS ; i += 1){
		placed_hedgehog_player[i] = 0 ;
	}
	while(placed_hedgehogs != N_PLAYERS*N_HEDGE){
		int player = rand()%N_PLAYERS ;
		if(placed_hedgehog_player[player] != N_HEDGE){
			int line = rand()%GAME_SIZE_Y ;
			placed_hedgehog_player[player] += 1 ;
			board_push(&b, line, 0, player + 'A') ;
			placed_hedgehogs += 1 ;
		}
	}

	b.current_player = 0 ;
	b.game_is_finished = false ;

	return b;
}

void board_free(board_t b)
{
	for (int i = 0; i < b.n_lines; i++)
	{
		for (int j = 0; j < b.n_rows; j++)
		{
			free(b.board[i][j]);
		}
		free(b.board[i]);
		free(b.n_hedge[i]);
		free(b.is_trapped[i]);
	}
	free(b.board);
	free(b.is_trapped);
	free(b.n_hedge);
	free(b.n_finished);
}

void board_push(board_t *b, int line, int row, char ctn)
{
	assert(line >= 0 && line < b->n_lines);
	assert(row >= 0 && row < b->n_rows);

	int i = b->n_hedge[line][row]; // position of the topmost element

	b->board[line][row][i] = ctn;

	b->n_hedge[line][row]++; // just added an element
}

char board_pop(board_t *b, int line, int row)
{
	assert(line >= 0 && line < b->n_lines);
	assert(row >= 0 && row < b->n_rows);

	int i = b->n_hedge[line][row];

	if (i >= 1)
	{
		b->n_hedge[line][row]--;
		return b->board[line][row][i-1];
	}

	return ' ';
}

// returns the number of hedgehogs in a specified block
int board_height(board_t *b, int line, int row)
{
	assert(line >= 0 && line < b->n_lines);
	assert(row >= 0 && row < b->n_rows);

	return b->n_hedge[line][row];
}

// returns the topmost team of a block
char board_top(board_t *b, int line, int row)
{
	assert(line >= 0 && line < b->n_lines);
	assert(row >= 0 && row < b->n_rows);

	int i = b->n_hedge[line][row];

	return i>0 ? b->board[line][row][i-1]: ' ';
}

// returns the team of a specific hedgehog
char board_peek(board_t *b, int line, int row, int pos)
{
	assert(line >= 0 && line < b->n_lines);
	assert(row >= 0 && row < b->n_rows);

	int i = b->n_hedge[line][row];

	if (pos < i)
	{

		return b->board[line][row][pos];
	}
	return ' ';
}

#include "engine.h"

int dice(void){
    return rand()%GAME_SIZE_Y +1 ;
}

int min(int a, int b){
    return(a < b ? a : b) ;
}

// changes the board
// BUG: le jeu termine pas
int play_turn(board_t* b){
    int dice_roll = dice() ;
    static int winning_player = 0 ;
    for(int i = 0 ; i < 2 ; i += 1){
        move m = scan_move(b, i, dice_roll) ;
        if(m.x1 == -1) continue;
        char player = board_pop(b, m.x1, m.y1) ;
        board_push(b, m.x2, m.y2, player) ;
        if(m.y2 == b -> n_rows-1){
            b -> n_finished[b -> current_player] += 1 ;
            if(b -> n_finished[b -> current_player] == min(3, N_HEDGE -2)){
                winning_player = b -> current_player ;
                b -> game_is_finished = true ;
            }
        }
    }
    if(b -> game_is_finished){
        if(b -> current_player == N_PLAYERS){
            return winning_player ;
        }
    }
    b -> current_player += 1 ;
    b -> current_player %= N_PLAYERS ;
    return -1 ;
}

// only reads what the inputs, do not change the board
// BUG : pas bons mouvements calculés
move scan_move(board_t* b, step s, int dice_roll){
	ui_render_board(b, 0);
    move m ;
    bool is_correct = false ;
    ui_printf("Player %c is playing. ", b -> current_player + 'A') ;
    ui_printf("Line you %s play horizontally: %d.\n", s == VERTICAL ? "will" : "", dice_roll) ;
    while(!is_correct){
        bool possible_move = false ;
        char print_board_top = ' ' ;
        if(s == VERTICAL){
            possible_move = false ;
            for(int i = 0 ; i < b -> n_lines ; i += 1){
                for(int j = 0 ; j < b -> n_rows-1 ; j += 1){
                    if((print_board_top = board_top(b, i, j)) == b -> current_player + 'A'){
                        possible_move = true ;
                        break ;
                    }
                    ui_printf("scanmove: i:%d, j:%d, board_top:%c, current_player:%c, possible_move:%d\n\n",
                    i, j, print_board_top, b -> current_player + 'A', possible_move) ;
                }
            }
            if(!possible_move){
                ui_printf("Sorry, no possible move for you here.\n") ;
                m.x1 = -1 ;
                return m ;
            }
        }
        if(s == HORIZONTAL){
            possible_move = false ;
            for(int j = 0 ; j < b -> n_rows-1 ; j += 1){
                if(board_height(b, dice_roll-1, j) != 0){
                    possible_move = true ;
                    break ;
                }
            }
            if(!possible_move){
                ui_printf("Sorry, no possible move for you here.\n") ;
                m.x1 = -1 ;
                return m ;
            }
        }
        ui_printf("(y%s) of the %s moving hedgehog.\n", s==VERTICAL ? ", x" : "", s== VERTICAL ? "vertically" : "horizontally") ;
        if(s == VERTICAL) ui_printf("Write skip if you want to skip.\n") ;
        char input[50];
        fflush(stdin) ;
        ui_scanf(" %49[^\n]", input);
        if(strcmp(input, "skip") == 0){
            if(possible_move && s == HORIZONTAL){
                ui_printf("Sorry, you cannot skip.\n") ;
                continue ;
            }
            else{
                ui_printf("You skipped.\n") ;
                m.x1 = -1 ;
                return m ;
            }
        }
        int i = 0 ;
        char x_input[5];
        char y_input[5];
        int ind = 0 ;
        bool is_sep = false ;
        while(input[i] != '\0'){
            if('0' <= input[i] && input[i] <= '9'){
                if(is_sep && s == VERTICAL){
                    x_input[ind] = input[i] ;
                }
                else{
                    y_input[ind] = input[i] ;
                }
                ind += 1 ;
            }
            else{
                if(ind > 0){
                    if(is_sep == true) break;
                    is_sep = true ;
                    ind = 0 ;
                }
            }
            i += 1;
        }
        unsigned int x_atoi = s==HORIZONTAL ? dice_roll : atoi(x_input) ;
        unsigned int y_atoi = atoi(y_input) ;
        if(y_atoi == 0 || (x_atoi == 0)){
            ui_printf("It seems that your input is not a number.\n") ;
            continue ;
        }
        if(x_atoi > (unsigned int) b -> n_lines || y_atoi > (unsigned int) b -> n_rows){
            ui_printf("It seems that your input is outside the board.\n") ;
            continue ;
        }
        x_atoi -= 1 ;
        y_atoi -= 1 ;
        m.x1 = x_atoi ;
        m.y1 = y_atoi ;
        if(board_height(b, x_atoi, y_atoi) == 0){
            ui_printf("It seems that there is no hedgehog in the block.\n") ;
            continue ;
        }
        if(b -> is_trapped[x_atoi][y_atoi]){
            bool found_hedgehog = false ;
            for(int j = 0 ; (unsigned int) j < y_atoi ; j += 1){
                if(board_height(b, x_atoi, j) > 0){
                    found_hedgehog = true ;
                    break ;
                }
            }
            if(found_hedgehog){
                ui_printf("You are trying to move a trapped hedgehog. The blocks before the hedgehog need to be emptied to move.\n") ;
                continue ;
            }
        }
        switch(s){
            case(HORIZONTAL):
                if(y_atoi == (unsigned int) b -> n_rows -1){
                    ui_printf("The hedgehog has already finished playing.\n") ;
                    continue ;
                }
                m.x2 = x_atoi ;
                m.y2 = y_atoi +1 ;
                break ;
            case(VERTICAL):
                m.y2 = y_atoi ;
                if(board_top(b, x_atoi, y_atoi) != b -> current_player + 'A'){
                    ui_printf("You are trying to move the hedgehog of another player.\n") ;
                    continue ;
                }
                char side ;
                ui_printf("Choose a direction to move the hedgehog : T for top or B for bottom.\n") ;
                ui_scanf(" %c", &side) ;
                switch(side){
                    case('H'):
                    case('h'):
                    case('T'):
                    case('t'):
                        if(x_atoi == 0){
                            ui_printf("You cannot move higher.\n") ;
                            continue ;
                        }
                        m.x2 = x_atoi - 1 ;
                        break ;
                    case('B'):
                    case('b'):
                        if(x_atoi == (unsigned int) b -> n_lines-1){
                            ui_printf("You cannot move lower.\n") ;
                            continue ;
                        }
                        m.x2 = x_atoi + 1 ;
                        break ;
                    default :
                        ui_printf("This is a wrong input.\n") ;
                        continue ;
                }
                break ;
        }
        is_correct=true;
    }
    return m ;
}

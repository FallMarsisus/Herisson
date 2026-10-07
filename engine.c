#include "engine.h"

int dice(){
    return rand()%GAME_SIZE_Y +1 ;
}

int min(int a, int b){
    return(a < b ? a : b) ;
}

int play_turn(board_t* b){
    int dice_roll = dice() ;
    for(int i = 0 ; i < 2 ; i += 1){
        move m = scan_move(b, i, dice_roll) ;
        if(m.x1 == -1) break;
        board_pop(b, m.x1, m.y1) ;
        board_push(b, m.x2, m.y2, b -> player + 'A') ;
        if(m.y2 == b -> n_rows-1){
            b -> n_finished[b -> player] += 1 ;
            if(b -> n_finished[b -> player] == min(3, N_HEDGE -2)){
                b -> game_is_finished = true ;
            }
        }
    }
    if(b -> game_is_finished){
        if(b -> player == N_PLAYERS){
            return 1 ;
        }
    }
    b -> player += 1 ;
    b -> player %= N_PLAYERS ;
    return 0 ;
}

move scan_move(board_t* b, step s, int dice_roll){
    move m ;
    bool is_correct = false ;
    printf("Player %c is playing.\n", b -> player + 'A') ;
    printf("%d\n",dice_roll) ;
    while(!is_correct){
        if(s == HORIZONTAL){
            bool possible_move = false ;
            for(int j = 0 ; j < b -> n_rows-1 ; j += 1){
                if(board_top(b, dice_roll-1, j) == b -> player + 'A'){
                    possible_move = true ;
                    break ;
                }
            }
            if(!possible_move){
                printf("Sorry, no possible move for you here.\n") ;
                m.x1 = -1 ;
                return m ;
            }
        }
        printf("(y%s) of the %s moving hedgehog.\n", s==VERTICAL ? ", x" : "", s== VERTICAL ? "vertically" : "horizontally") ;
        char input[50];
        fflush(stdin) ;
        fgets(input, sizeof(input), stdin) ;
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
        unsigned int x_atoi = s==HORIZONTAL ? dice_roll+1 : atoi(x_input) ;
        unsigned int y_atoi = atoi(y_input) ;
        if(y_atoi == 0 || (x_atoi == 0)){
            printf("It seems that your input is not a number.\n") ;
            continue ;
        }
        if(x_atoi >= b -> n_lines || y_atoi >= b -> n_rows){
            printf("It seems that your input is outside the board.\n") ;
            continue ;
        }
        x_atoi -= 1 ;
        y_atoi -= 1 ;
        m.x1 = x_atoi ;
        m.y1 = y_atoi ;
        if(board_height(b, x_atoi, y_atoi) == 0){
            printf("atoi: %d, %d\n", x_atoi, y_atoi) ;
            printf("It seems that there is no hedgehog in the block.\n") ;
            continue ;
        }
        if(b -> is_trapped[x_atoi][y_atoi]){
            bool found_hedgehog = false ;
            for(int j = 0 ; j < y_atoi ; j += 1){
                if(board_height(b, x_atoi, j) > 0){
                    found_hedgehog = true ;
                    break ;
                }
            }
            if(found_hedgehog){
                printf("You are trying to move a trapped hedgehog. The blocks before the hedgehog need to be emptied to move.\n") ;
                continue ;
            }
        }
        switch(s){
            case(HORIZONTAL):
                if(y_atoi == b -> n_rows -1){
                    printf("The hedgehog has already finished playing.\n") ;
                    continue ;
                }
                m.x2 = x_atoi ;
                m.y2 = y_atoi +1 ;
                break ;
            case(VERTICAL):
                m.y2 = y_atoi ;
                if(board_top(b, x_atoi, y_atoi) != b -> player + 'A'){
                    printf("You are trying to move the hedgehog of another player.\n") ;
                    continue ;
                }
                char side ;
                printf("Choose a direction to move the hedgehog : T for top or B for bottom.\n") ;
                scanf("%c", &side) ;
                printf("coord: %d, %d, mouv: %c\n", x_atoi, y_atoi, side) ;
                switch(side){
                    case('H'):
                    case('h'):
                    case('T'):
                    case('t'):
                        if(x_atoi == 0){
                            printf("You cannot move higher.\n") ;
                            continue ;
                        }
                        m.x2 = x_atoi - 1 ;
                        break ;
                    case('B'):
                    case('b'):
                        if(x_atoi == b -> n_lines-1){
                            printf("You cannot move lower.\n") ;
                            continue ;
                        }
                        m.x2 = x_atoi + 1 ;
                        break ;
                    default :
                        printf("This is a wrong input.\n") ;
                        continue ;
                }
                break ;
        }
        is_correct=true;
    }
    return m ;
}

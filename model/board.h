#pragma once
#include "coordinates_list.h"

typedef struct Board {
    char grid[8][8];
    char white_num;
    char grey_num;
    char turn;
    int no_eat_counter;
    int has_moves;
} Board;


Board initBoard();
int get_white_num(Board board);
int get_grey_num(Board board);
void change_turn(Board* board);
char is_game_over(Board* board);
char get_cat(Board* board, coordinates coord);
int check_cat(Board* board, coordinates coords);
int remove_cat(Board* board, coordinates coords);
int make_king(Board* board, coordinates coords);
int add_cat(Board* board, char cat, coordinates coords);
int can_move(Board board, coordinates coords1, coordinates coords2);
coordinates_list get_moves(Board board, coordinates coord);
void print_board(Board board);

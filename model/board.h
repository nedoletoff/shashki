#pragma once
#include "coordinates_list.h"

typedef struct Board {
    char grid[8][8];
    char white_num;
    char grey_num;
} Board;


Board initBoard();
int get_white_num(Board board);
int get_grey_num(Board board);
char get_cat(Board board, coordinates coord);
int check_cat(Board board, coordinates coords);
int remove_cat(Board* board, coordinates coords);
int make_king(Board* board, coordinates coords);
int add_cat(Board* board, char cat, coordinates coords);
int can_move(Board board, coordinates coords1, coordinates coords2);
void print_board(Board board);

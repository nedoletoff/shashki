#pragma once
#include "coordinates_list.h"


typedef struct Board {
    char grid[8][8];
    char white_num;
    char grey_num;
    char turn;
    char eaten;
    int no_eat_counter;
    int has_moves;
} Board;

void change_turn(Board* board);
char lower_case(char);
char get_cat(Board* board, coordinates coord);
int is_able_to_move(Board* board, coordinates cords);
int check_cat(Board* board, coordinates cords);
int disable_cat(Board* board, coordinates cords);
int remove_cat(Board* board, coordinates cords);
int make_king(Board* board, coordinates cords);
int add_cat(Board* board, char cat, coordinates cords);
int eat(Board* board, coordinates first, coordinates end);
int move(Board* board, coordinates first, coordinates end);

Board initBoard();
void initB(Board*);

void get_movable_cat(Board* board, coordinates_list* moves, coordinates_list* eats);
void get_moves(Board* board, coordinates cords, coordinates_list* list);
char is_game_over(Board* board);
int get_white_num(Board board);
int get_grey_num(Board board);
void change_turn_and_finally_eat(Board* board);
int move_cat(Board* board, coordinates cords1, coordinates cords2);
void print_board(Board* board);
void get_copy(Board* self, Board* other);
Board get_Board(Board*);


#include <stdio.h>
#include "board.h"


Board initBoard() {
    Board res;
    res.white_num = 12;
    res.grey_num = 12;
    for (char i = 0; i < 8; i++)
        for (char j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0 && (i < 3))
                res.grid[i][j] = 'g';
            else if ((i + j) % 2 == 0 && i > 4)
                res.grid[i][j] = 'w';
            else
                res.grid[i][j] = ' ';
        }
    return res;
}

int get_white_num(Board board) {
    return board.white_num;
}

int get_grey_num(Board board) {
    return board.grey_num;
}

char get_cat(Board board, coordinates coords) {
    if (coords.height < 0 || coords.height >= 8) {
        printf("Wrong coords.height - %d\n", coords.height);
        return 0;
    }
    if (coords.width < 0 || coords.width >= 8) {
        printf("Wrong coords.width - %d\n", coords.width);
        return 0;
    }

    return board.grid[coords.height][coords.width];
}

int check_cat(Board board, coordinates coords) {
    char cat = get_cat(board, coords);
    if (cat == 'W' || cat == 'w') {
        return 1;
    }
    else if (cat == 'G' || cat == 'g') {
        return 1;
    }
    else
        printf("cell %d - coords.height, %d - coords.width doesnt contain cat\n",
         coords.height, coords.width);
    return 0;
}

int remove_cat(Board* board, coordinates coords) {
    char cat = get_cat(*board, coords);

    if (!check_cat(*board, coords))
        return 1;
    if (cat == 'W' || cat == 'w') {
        board->white_num--;
        board->grid[coords.height][coords.width] = ' ';
        return 0;
    }
    else if (cat == 'G' || cat == 'g') {
        board->grey_num--;
        board->grid[coords.height][coords.width] = ' ';
        return 0;
    }
    return 1;
}

int make_king(Board* board, coordinates coords) {
    char cat = get_cat(*board, coords);
    if (cat == 'w' && coords.height == 0)
        board->grid[coords.height][coords.width] = 'W';
    else if (cat == 'g' && coords.height == 7)
        board->grid[coords.height][coords.width] = 'G';
    else {
        printf("Cant make king\n");
        return 1;
    }
    return 0;
}



int add_cat(Board* board, char cat, coordinates coords) {
    if ((coords.height + coords.width) % 2) {
        printf("Not brown cell\n");
        return 2;
    }
    if (cat == 'W' || cat == 'w')
        board->white_num++;
    else if (cat == 'G' || cat == 'g')
        board->grey_num++;
    else {
        printf("Wrong cat\n");
        return 3;
    }
    board->grid[coords.height][coords.width] = cat;
    return 0;
}

int can_move(Board board, coordinates coords1, coordinates coords2) {
    if (!check_cat(board, coords1))
        return 1;

    char my_cat = get_cat(board, coords1);
    if (my_cat == 0)
        return 1;
    if (my_cat == 'g') {

    }
}

int move_cat(Board* board, coordinates coords1, coordinates coords2) {
    char cat = get_cat(*board, coords1);
    int res = remove_cat(board, coords1);
    if (res)
        return res;
    return add_cat(board, cat, coords2);
}

void print_board(Board board) {
    printf("%d - whites, %d - grays\n", board.white_num, board.grey_num);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%c", board.grid[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Board board = initBoard();
    printf("Board init\n");
    print_board(board);
    printf("Board print\n");
    coordinates coord = {1, 3};
    coordinates coord1 = {4, 4};
    coordinates coord2 = {0, 0};
    move_cat(&board, coord, coord1);
    printf("Board move_cat\n");
    print_board(board);
    printf("Board print\n");
    make_king(&board, coord2);
    print_board(board);
    printf("Board print\n");

    return 0;
}

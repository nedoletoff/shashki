#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "coordinates_list.h"


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

char get_cat(Board board, coordinates cords) {
    if (cords.height < 0 || cords.height >= 8) {
        printf("Wrong cords.height - %d\n", cords.height);
        return 0;
    }
    if (cords.width < 0 || cords.width >= 8) {
        printf("Wrong cords.width - %d\n", cords.width);
        return 0;
    }

    return board.grid[cords.height][cords.width];
}

int check_cat(Board board, coordinates cords) {
    char cat = get_cat(board, cords);
    if (cat == 'W' || cat == 'w') {
        return 1;
    }
    else if (cat == 'G' || cat == 'g') {
        return 1;
    }
    else
        printf("cell %d - cords.height, %d - cords.width doesnt contain cat\n",
         cords.height, cords.width);
    return 0;
}

int check_coordinates(coordinates coords) {
    if (coords.height >= 8 || coord.height < 0)
        return 1;

    if (coords.width >= 8 || coord.width < 0)
        return 1;
    return 0;
}

int remove_cat(Board* board, coordinates cords) {
    char cat = get_cat(*board, cords);

    if (!check_cat(*board, cords))
        return 1;
    if (cat == 'W' || cat == 'w') {
        board->white_num--;
        board->grid[cords.height][cords.width] = ' ';
        return 0;
    }
    else if (cat == 'G' || cat == 'g') {
        board->grey_num--;
        board->grid[cords.height][cords.width] = ' ';
        return 0;
    }
    return 1;
}

int make_king(Board* board, coordinates cords) {
    char cat = get_cat(*board, cords);
    if (cat == 'w' && cords.height == 0)
        board->grid[cords.height][cords.width] = 'W';
    else if (cat == 'g' && cords.height == 7)
        board->grid[cords.height][cords.width] = 'G';
    else {
        printf("Cant make king\n");
        return 1;
    }
    return 0;
}



int add_cat(Board* board, char cat, coordinates cords) {
    if (check_coordinates(cords)) {
        printf("Out of board\n");
        return 4;
    if ((cords.height + cords.width) % 2) {
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
    board->grid[cords.height][cords.width] = cat;
    return 0;
    }
}

int usual_eat(Board* board, coordinates first, coordinates end) {
    if (abs(first.height - end.height) != 2) {
        printf("Cant eat");
        return 1;
    }
    else {
        coordinates enemy;
        if (first.width > end.width && first.height > end.height) {
            enemy.height = first.height - 1;
            enemy.width =  first.width - 1;
        }
        else if (first.width > end.width && first.height < end.height) {
            enemy.height = first.height + 1;
            enemy.width =  first.width - 1;
        }
        else if (first.width < end.width && first.height > end.height) {
            enemy.height = first.height - 1;
            enemy.width =  first.width + 1;
        }
        else if (first.width < end.width && first.height < end.height) {
            enemy.height = first.height + 1;
            enemy.width =  first.width + 1;
        }

        remove_cat(board, enemy);
    }
    return 0;
}

int move(Board* board, coordinates first, coordinates end) {

}


int usual_can_move(Board board, coordinates cords1, coordinates cords2, coordinates_list* res) {
    if (check_coordinates(cords1))
        return 1;
    if (check_coordinates(cords2))
        return 1;
    if (!check_cat(board, cords1))
        return 1;

    coordinates_list list;
    init(&list);

    char my_cat = get_cat(board, cords1);
    if (my_cat == 0 || my_cat == ' ')
        return 1;
    if (my_cat == 'g') {
    }
}

int move_cat(Board* board, coordinates cords1, coordinates cords2) {
    char cat = get_cat(*board, cords1);
    int res = remove_cat(board, cords1);
    if (res)
        return res;
    return add_cat(board, cat, cords2);
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

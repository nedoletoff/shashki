#include <stdio.h>


typedef struct Board {
    char grid[8][8];
    char white_num;
    char grey_num;
} Board;

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

char get_cat(Board board, int height, int width) {
    if (height < 0 || height >= 8) {
        printf("Wrong height - %d\n", height);
        return 0;
    }
    if (width < 0 || width >= 8) {
        printf("Wrong width - %d\n", width);
        return 0;
    }

    return board.grid[height][width];
}

int check_cat(Board board, int height, int width) {
    char cat = get_cat(board, height, width);
    if (cat == 'W' || cat == 'w') {
        return 1;
    }
    else if (cat == 'G' || cat == 'g') {
        return 1;
    }
    else
        printf("cell %d - height, %d - width doesnt contain cat\n",
         height, width);
    return 0;
}

int remove_cat(Board* board, int height, int width) {
    char cat = get_cat(*board, height, width);

    if (!check_cat(*board, height, width))
        return 1;
    if (cat == 'W' || cat == 'w') {
        board->white_num--;
        board->grid[height][width] = ' ';
        return 0;
    }
    else if (cat == 'G' || cat == 'g') {
        board->grey_num--;
        board->grid[height][width] = ' ';
        return 0;
    }
}

int make_king(Board* board, int height, int width) {
    char cat = get_cat(*board, height, width);
    if (cat == 'w' && height == 0)
        board->grid[height][width] = 'W';
    else if (cat == 'g' && height == 7)
        board->grid[height][width] = 'G';
    else {
        printf("Cant make king\n");
        return 1;
    }
    return 0;
}



int add_cat(Board* board, char cat, int height, int width) {
    if ((height + width) % 2) {
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
    board->grid[height][width] = cat;
    return 0;
}

int can_move(Board board, int height1, int width1, int height2, int width2) {
    if (!check_cat(board, height1, width1))
        return 1;

    char my_cat = get_cat(board, height1, width1);
    if (my_cat == 0)
        return 1;
    if (my_cat == 'g') {

    }

}

int move_cat(Board* board, int height1, int width1, int height2, int width2) {
    char cat = get_cat(*board, height1, width1);
    int res = remove_cat(board, height1, width1);
    if (res)
        return res;
    return add_cat(board, cat, height2, width2);
}

void print(Board board) {
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
    print(board);
    printf("Board print\n");
    move_cat(&board, 1, 3, 4, 4);
    printf("Board move_cat\n");
    print(board);
    printf("Board print\n");
    make_king(&board, 0, 0);
    print(board);
    printf("Board print\n");
}

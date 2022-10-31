#include <stdio.h>


typedef struct Board {
    char[8][8] grid;
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
                res.grid[j][i] = 'W';
            else if ((i + j) % 2 == 0 && i > 4)
                res.grid[j][i] = 'G';
            else
                res.grid[j][i] = 0;
        }
    return res;
}

int get_white_num(Board* board) {
    return board.white_num;
}

int get_gray_num(Board* board) {
    return board.gray_num;
}

void remove_cat(Board* board, int width, int height) {
    char cat = board.grid[width][height];
    if (char == 'W') {
        white_num--;
        board.grid[width][height] = 0;
    }
    else if (char == 'G') {
        grey_num--;
        board.grid[width][width] = 0;
    }
    else
        printf("cell %d - width, %d - height doesnt contain cat",
         width, height);
}

void print(Board* board) {
    printf("%d - whites, %d - grays", board.white_num, board.grey_num);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf(board[i][j]);
        }
        printf('\n');
    }
}

int main() {

}
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "coordinates_list.h"


Board initBoard() {
    Board res;
    res.no_eat_counter = 0;
    res.turn = 'w';
    res.white_num = 12;
    res.grey_num = 12;
    res.eaten = 0;
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

void initB(Board* board) {
    board->no_eat_counter = 0;
    board->turn = 'w';
    board->white_num = 12;
    board->grey_num = 12;
    board->eaten = 0;

    for (char i = 0; i < 8; i++)
        for (char j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0 && (i < 3))
                board->grid[i][j] = 'g';
            else if ((i + j) % 2 == 0 && i > 4)
                board->grid[i][j] = 'w';
            else
                board->grid[i][j] = ' ';
        }
}

char upper_case(char val) {
    if (val < 'A' && val >= 'a')
        return val + ('A' - 'a');
    else val;
}

char lower_case(char val) {
    if (val > 'a' && val <= 'Z')
        return val - ('A' - 'a');
    return val;
}

char get_enemy(char val) {
    if (val == 'g' || val == 'G')
        return 'w';
    else if (val == 'w' || val == 'W')
        return 'g';
    return '\0';
}

int get_white_num(Board board) {
    return board.white_num;
}

int get_grey_num(Board board) {
    return board.grey_num;
}

char is_game_over(Board* board) {
    if (board->has_moves == 0) {
        change_turn(board);
        board->no_eat_counter--;
        return board->turn;
    }
    if (get_white_num(*board) == 0)
        return 'w';
    if (get_grey_num(*board)== 0)
        return 'g';
    if (board->no_eat_counter == 29)
        return 'o';
    return 'n';
}

void change_turn(Board* board) {
    board->no_eat_counter++;
    if (board->turn == 'w')
        board->turn = 'g';
    else
        board->turn = 'w';
}

char get_cat(Board* board, coordinates cords) {
    if (cords.height < 0 || cords.height >= 8) {
        //printf("Wrong cords.height - %d\n", cords.height);
        //printf("Wrong cords.width - %d\n", cords.width);
        return 0;
    }
    if (cords.width < 0 || cords.width >= 8) {
        //printf("Wrong cords.width - %d\n", cords.width);
        //printf("Wrong cords.height - %d\n", cords.height);
        return 0;
    }

    return board->grid[cords.height][cords.width];
}

int check_cat(Board* board, coordinates cords) {
    char cat = get_cat(board, cords);
    if (cat == 'W' || cat == 'w') {
        return 1;
    }
    else if (cat == 'G' || cat == 'g') {
        return 1;
    }
    else if (cat == 'e') {
        //printf("Cat height - %d, width - %d should be deleted in next move\n",
        // cords.height, cords.width);
        return 1;
    }
    else
        printf("cell %d - cords.height, %d - cords.width doesnt contain cat\n",
         cords.height, cords.width);
    return 0;
}

int check_coordinates(coordinates cords) {
    if (cords.height >= 8 || cords.height < 0)
        return 1;

    if (cords.width >= 8 || cords.width < 0)
        return 1;
    return 0;
}

int remove_cat(Board* board, coordinates cords) {
    char cat = get_cat(board, cords);

    if (!check_cat(board, cords))
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
    else if (cat == 'e') {
        board->grid[cords.height][cords.width] = ' ';
        return 0;
    }
    return 1;
}

int disable_cat_int_int(Board* board, int h, int w) {
    coordinates cords = {w, h};
    return disable_cat(board, cords);
}

int disable_cat(Board* board, coordinates cords) {
    char cat = get_cat(board, cords);

    if (!check_cat(board, cords))
        return 1;
    if (cat == 'W' || cat == 'w') {
        board->white_num--;
        board->grid[cords.height][cords.width] = 'e';
        board->eaten = 15;
        return 0;
    }
    else if (cat == 'G' || cat == 'g') {
        board->grey_num--;
        board->grid[cords.height][cords.width] = 'e';
        board->eaten = 15;
        return 0;
    }
    return 1;
}

int can_make_king(Board* board, coordinates cords) {
    char cat = get_cat(board, cords);
    if (cat == 'w' && cords.height == 0 || cat == 'g' && cords.height == 7)
        return 1;
    return 0;
}

int make_king(Board* board, coordinates cords) {
    char cat = get_cat(board, cords);
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
    }
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

int is_king_able_to_move(Board* board, coordinates cords) {
    printf("start func is_king_able_to_move\n");
    char cat = get_cat(board, cords);
    char enemy = get_enemy(cat);
    printf("king enemy - %c", enemy);
    int counter = 0;
    coordinates cur;
    cur.height = cords.height;
    cur.width = cords.width;
    while (cur.height < 8 && cur.width < 8) {
        cur.height++;
        cur.width++;
        if (lower_case(get_cat(board, cur)) == enemy) {
            cur.height++;
            cur.width++;
            if (get_cat(board, cur) == ' ')
                return 1;
            else break;
        }
        else if (lower_case(get_cat(board, cur)) == lower_case(cat))
            break;
        counter++;
    }
    cur.height = cords.height;
    cur.width = cords.width;
    while (cur.height >= 0 && cur.width < 8) {
        cur.height--;
        cur.width++;
        if (lower_case(get_cat(board, cur)) == enemy) {
            cur.height--;
            cur.width++;
            if (get_cat(board, cur) == ' ')
                return 1;
            else break;
        }
        else if (lower_case(get_cat(board, cur)) == lower_case(cat))
            break;
        counter++;
    }
    cur.height = cords.height;
    cur.width = cords.width;
    while (cur.height < 8 && cur.width >= 0) {
        cur.height++;
        cur.width--;
        if (lower_case(get_cat(board, cur)) == enemy) {
            cur.height++;
            cur.width--;
            if (get_cat(board, cur) == ' ')
                return 1;
            else break;
        }
        else if (lower_case(get_cat(board, cur)) == lower_case(cat))
            break;
        counter++;
    }
    cur.height = cords.height;
    cur.width = cords.width;
    while (cur.height >= 0 && cur.width >= 0) {
        cur.height--;
        cur.width--;
        if (lower_case(get_cat(board, cur)) == enemy) {
            cur.height--;
            cur.width--;
            if (get_cat(board, cur) == ' ')
                return 1;
            else break;
        }
        else if (lower_case(get_cat(board, cur)) == lower_case(cat))
            break;
        counter++;
    }
    if (counter > 0)
        return 2;

    printf("king cannot move\n");
    return 0;

}

int is_usual_able_to_move(Board* board, coordinates cords) {
    char cat = get_cat(board, cords);
    char enemy = get_enemy(cat);
    coordinates cur;
    int default_coef;
    if (cat == 'w')
        default_coef = 1;
    else if (cat == 'g')
        default_coef = -1;
    else
        return 0;

    cur.height = cords.height - 1 * default_coef;
    cur.width = cords.width - 1;
    if (lower_case(get_cat(board, cur)) == enemy) {
        cur.height = cur.height - 1 * default_coef;
        cur.width--;
        if (get_cat(board, cur) == ' ')
            return 1;
    }
    cur.height = cords.height - 1 * default_coef;
    cur.width = cords.width + 1;
    if (lower_case(get_cat(board, cur)) == enemy) {
        cur.height = cur.height - 1 * default_coef;
        cur.width++;
        if (get_cat(board, cur) == ' ')
            return 1;
    }
    cur.height = cords.height + 1 * default_coef;
    cur.width = cords.width - 1;
    if (lower_case(get_cat(board, cur)) == enemy) {
        cur.height = cur.height + 1 * default_coef;
        cur.width--;
        if (get_cat(board, cur) == ' ')
            return 1;
    }
    cur.height = cords.height + 1 * default_coef;
    cur.width = cords.width + 1;
    if (lower_case(get_cat(board, cur)) == enemy) {
        cur.height = cur.height + 1 * default_coef;
        cur.width++;
        if (get_cat(board, cur) == ' ')
            return 1;
    }

    cur.height = cords.height - 1 * default_coef;
    cur.width = cords.width - 1;
    if (get_cat(board, cur) == ' ') {
        //printf("%d - cur.height, %d - cur.width, %c - cat\n",
        //cur.height, cur.width, get_cat(board, cur));
        return 2;
    }

    cur.height = cords.height - 1 * default_coef;
    cur.width = cords.width + 1;
    if (get_cat(board, cur) == ' ') {
        //printf("%d - cur.height, %d - cur.width, %c - cat\n",
        //cur.height, cur.width, get_cat(board, cur));
        return 2;
    }

    return 0;
}

int is_able_to_move(Board* board, coordinates cords) {
    char cat = get_cat(board, cords);
    if (cat != 'w' && cat != 'g') {
        printf("%c, %d - cat\t", cat, cat);
        printf("%d - height, %d - width\n", cords.height, cords.width);
    }
    if (cat == 'W' || cat == 'G') {
        return is_king_able_to_move(board, cords);
    }
    else
        return is_usual_able_to_move(board, cords);
}

void get_king_moves(Board* board, coordinates cords, coordinates_list* list) {
    destroy(list);
    initCL(list);

    char cat = get_cat(board, cords);
    char enemy = get_enemy(cat);
    coordinates cur;

    int mode = is_king_able_to_move(board, cords);

    cur.height = cords.height;
    cur.width = cords.width;
    while (cur.height < 8 && cur.width < 8) {
        cur.height++;
        cur.width++;
        if (lower_case(get_cat(board, cur)) == enemy) {
            cur.height++;
            cur.width++;
            if (get_cat(board, cur) == ' ')
                push_back(list, cur);
            break;
        }
        else if (lower_case(get_cat(board, cur)) == lower_case(cat))
            break;
        if (mode == 2)
            push_back(list, cur);
    }
    cur.height = cords.height;
    cur.width = cords.width;
    while (cur.height >= 0 && cur.width < 8) {
        cur.height--;
        cur.width++;
        if (lower_case(get_cat(board, cur)) == enemy) {
            cur.height--;
            cur.width++;
            if (get_cat(board, cur) == ' ')
                push_back(list, cur);
            break;
        }
        else if (lower_case(get_cat(board, cur)) == lower_case(cat))
            break;
        if (mode == 2)
            push_back(list, cur);
    }
    cur.height = cords.height;
    cur.width = cords.width;
    while (cur.height < 8 && cur.width >= 0) {
        cur.height++;
        cur.width--;
        if (lower_case(get_cat(board, cur)) == enemy) {
            cur.height++;
            cur.width--;
            if (get_cat(board, cur) == ' ')
                push_back(list, cur);
            break;
        }
        else if (lower_case(get_cat(board, cur)) == lower_case(cat))
            break;
        if (mode == 2)
            push_back(list, cur);
    }
    cur.height = cords.height;
    cur.width = cords.width;
    while (cur.height >= 0 && cur.width >= 0) {
        cur.height--;
        cur.width--;
        if (lower_case(get_cat(board, cur)) == enemy) {
            cur.height--;
            cur.width--;
            if (get_cat(board, cur) == ' ')
                push_back(list, cur);
            break;
        }
        else if (lower_case(get_cat(board, cur)) == lower_case(cat))
            break;
        if (mode == 2)
            push_back(list, cur);
    }
}

void get_usual_moves(Board* board, coordinates cords, coordinates_list* list) {
    destroy(list);
    initCL(list);

    char cat = get_cat(board, cords);
    char enemy = get_enemy(cat);
    coordinates cur;
    int default_coef;
    if (cat == 'w')
        default_coef = 1;
    else if (cat == 'g')
        default_coef = -1;
    else
        return;

    cur.height = cords.height - 1 * default_coef;
    cur.width = cords.width - 1;
    if (lower_case(get_cat(board, cur)) == enemy) {
        cur.height = cur.height - 1 * default_coef;
        cur.width--;
        if (get_cat(board, cur) == ' ')
            push_back(list, cur);
    }
    cur.height = cords.height - 1 * default_coef;
    cur.width = cords.width + 1;
    if (lower_case(get_cat(board, cur)) == enemy) {
        cur.height = cur.height - 1 * default_coef;
        cur.width++;
        if (get_cat(board, cur) == ' ')
            push_back(list, cur);
    }
    cur.height = cords.height + 1 * default_coef;
    cur.width = cords.width - 1;
    if (lower_case(get_cat(board, cur)) == enemy) {
        cur.height = cur.height + 1 * default_coef;
        cur.width--;
        if (get_cat(board, cur) == ' ')
            push_back(list, cur);
    }
    cur.height = cords.height + 1 * default_coef;
    cur.width = cords.width + 1;
    if (lower_case(get_cat(board, cur)) == enemy) {
        cur.height = cur.height + 1 * default_coef;
        cur.width++;
        if (get_cat(board, cur) == ' ')
            push_back(list, cur);
    }

    if (list->size == 0) {
        cur.height = cords.height - 1 * default_coef;
        cur.width = cords.width - 1;
        if (get_cat(board, cur) == ' ')
            push_back(list, cur);

        cur.height = cords.height - 1 * default_coef;
        cur.width = cords.width + 1;
        if (get_cat(board, cur) == ' ')
            push_back(list, cur);
    }
}

void get_moves(Board* board, coordinates cords, coordinates_list* list) {
    char cat = get_cat(board, cords);
    if (cat == 'W' || cat == 'G')
        get_king_moves(board, cords, list);
    else if (cat == 'w' || cat == 'g')
        get_usual_moves(board, cords, list);
    else
        printf("SOMETHING WRONG IN MAIN GET MOVES\n");
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

        disable_cat(board, enemy);
        move_cat(board, first, end);
    }
    return 0;
}

int king_eat(Board* board, coordinates first, coordinates end) {
    int h_min; int w_min;
    int h_max; int w_max;
    if (first.height > end.height) {
        h_max = first.height;
        h_min = end.height;
    }
    else if (first.height < end.height) {
        h_max = end.height;
        h_min = first.height;
    }
    else {
        printf("King moving on itself\n");
        return 1;
    }

    if (first.width > end.width) {
        w_max = first.width;
        w_min = end.width;
    }
    else if (first.width < end.width) {
        w_max = end.width;
        w_min = first.width;
    }
    else {
        printf("King moving on itself\n");
        return 1;
    }

    while (h_max != h_min && w_max != w_min)
        disable_cat_int_int(board, h_min++, w_min++);
    move_cat(board, first, end);
    return 0;
}

int eat(Board* board, coordinates first, coordinates end) {
    char cat = get_cat(board, first);
    if (cat == 'W' || cat == 'G')
        return king_eat(board, first, end);
    if (cat == 'w' || cat == 'g')
        return usual_eat(board, first, end);

    printf("CANT EAT IN MAIN EAT\n");
    return -1;
}

void change_turn_and_finally_eat(Board* board) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (board->grid[i][j] == 'e') {
                coordinates cords = {i, j};
                remove_cat(board, cords);
            }
    change_turn(board);
    if (board->eaten != 0) {
        board->no_eat_counter = 0;
        board->eaten = 0;
    }
}

int move(Board* board, coordinates first, coordinates end) {
    if (is_able_to_move(board, first) == 1) {
        eat(board, first, end);
    }
    if (is_able_to_move(board, first) == 2)
        move_cat(board, first, end);

    if (can_make_king(board, end))
        make_king(board, end);
    return 0;
}

int move_cat(Board* board, coordinates cords1, coordinates cords2) {
    char cat = get_cat(board, cords1);
    int res = remove_cat(board, cords1);
    if (res)
        return res;
    return add_cat(board, cat, cords2);
}

void print_board(Board* board) {
    printf("%d - whites, %d - grays\n %c - turn\n", board->white_num, board->grey_num, board->turn);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%c", board->grid[i][j]);
        }
        printf("\n");
    }
}

void get_movable_cat(Board* board, coordinates_list* moves, coordinates_list* eats) {
    destroy(eats);
    initCL(eats);
    destroy(moves);
    initCL(moves);
    int check = 1;
    board->has_moves = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            coordinates cords = {i, j};
            if (board->turn == get_cat(board, cords) ||
                board->turn == lower_case(get_cat(board, cords))) {

                switch (is_able_to_move(board, cords)) {
                case 1:
                    push_back(eats, cords);
                    board->has_moves = 1;
                    break;
                case 2:
                    push_back(moves, cords);
                    board->has_moves = 1;
                    break;
                }

            }
        }
    if (eats->size > 0) {
        //printf("eats not zero\n");
        destroy(moves);
        initCL(moves);
    }
    //printf("eats - %d, moves - %d\n", eats->size, moves->size);

}

int main() {
    Board board = initBoard();
    printf("Board init\n");
    print_board(&board);
    printf("Board print\n");
    coordinates coord = {1, 3};
    coordinates coord1 = {4, 4};
    coordinates coord2 = {0, 0};
    move_cat(&board, coord, coord1);
    printf("Board move_cat\n");
    print_board(&board);
    printf("Board print\n");
    make_king(&board, coord2);
    print_board(&board);
    printf("Board print\n");

    return 0;
}

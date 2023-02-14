#include "ai.h"
#include <stdio.h>
#include <stdlib.h>
long long count;
const int max_size = 3;

int get_score(Board* board) {
    int score = 0;
    coordinates_list* list = (coordinates_list*) malloc(sizeof(coordinates_list));
    initCL(list);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            {
                coordinates cur = {i, j};
                char cat = get_cat(board, cur);
                if (lower_case(cat) == board->turn) {
                    get_moves(board, cur, list);
                    if (is_able_to_move(board, cur) == 1) {
                        score += list->size * 10;
                        //TODO add function counter eats
                    }
                    else if (is_able_to_move(board, cur) == 2)
                        score += list->size;
                }
            }
    return score;
}

int get_best_moves(Board* board, int deep, char color) {
    // вспомогательные
    char that_turn = board->turn;
    int max_score = -1;
    int arg_max_score;
    int max_scores[] = {-1, -1, -1};
    if (deep <= 0) {
        return get_score(board);
    }
    if (is_game_over(board) == color) {
        return 1000;
    }

    Board* cur_board = (Board*) malloc(sizeof(Board));

    // изменяемые
    Board* mas_boards[max_size];
    for (int i = 0; i < max_size; i++)
        mas_boards[i] = (Board*) malloc(sizeof(Board));
    int size = 0;

    // временные
    int cur_score = -1;
    coordinates cur_movable_cat;
    coordinates cur_move;
    coordinates_list* moves_cur_cat = (coordinates_list*) malloc(sizeof(coordinates_list));
    initCL(moves_cur_cat);
    coordinates_list* mov_on_turn = (coordinates_list*) malloc(sizeof(coordinates_list));
    initCL(mov_on_turn);
    coordinates_list* eat_on_turn = (coordinates_list*) malloc(sizeof(coordinates_list));
    initCL(eat_on_turn);

    // шашки, которые могут двигаться
    get_movable_cat(board, mov_on_turn, eat_on_turn);

    // ищем ходы без съедения
    while (mov_on_turn->size > 0) {
        cur_score = -1;
        pop_back(mov_on_turn, &cur_movable_cat);
        get_moves(board, cur_movable_cat, moves_cur_cat);
        while (moves_cur_cat->size > 0) {
            get_copy(board, cur_board);
            pop_back(moves_cur_cat, &cur_move);
            move(cur_board, cur_movable_cat, cur_move);
            change_turn_and_finally_eat(cur_board);
            cur_score = get_score(cur_board);
            if (size < max_size) {
                mas_boards[size] = cur_board;
                max_scores[size++] = cur_score;
            }
            else {
                for (int i = 0; i < max_size; i++) {
                    if (cur_score > max_scores[i]) {
                        mas_boards[i] = cur_board;
                        max_scores[i] = cur_score;
                        break;
                    }
                }
            }
        }
    }

    // ищем ходы со съедением
    while (eat_on_turn->size > 0) {
        cur_score = -1;
        pop_back(eat_on_turn, &cur_movable_cat);
        get_moves(board, cur_movable_cat, moves_cur_cat);
        while (moves_cur_cat->size > 0) {

            coordinates_list* temp_moves = (coordinates_list*) malloc(sizeof(coordinates_list));
            initCL(temp_moves);
            get_copy(board, cur_board);
            pop_back(moves_cur_cat, &cur_move);
            push_back(temp_moves, cur_move);
            move(cur_board, cur_movable_cat, cur_move);
            //используем все возможные ходы
            while (is_able_to_move(cur_board, cur_move) == 1) {
                coordinates temp_cat;
                temp_cat.height = cur_move.height;
                temp_cat.width = cur_move.width;

                coordinates_list* t_moves = (coordinates_list*) malloc(sizeof(coordinates_list));
                initCL(t_moves);
                get_moves(cur_board, temp_cat, t_moves);
                pop_back(t_moves, &cur_move);
                push_back(temp_moves, cur_move);
                move(cur_board, temp_cat, cur_move);
            }
            change_turn_and_finally_eat(cur_board);

            cur_score = get_score(cur_board);
            if (size < max_size) {
                mas_boards[size] = cur_board;
                max_scores[size++] = cur_score;
            }
            else {
                char check = 1;
                for (int i = 0; i < max_size; i++) {
                    if (cur_score > max_scores[i]) {
                        check = 0;
                        max_scores[i] = cur_score;
                        mas_boards[i] = cur_board;
                        break;
                    }
                }
                if (check) {
                destroy(temp_moves);
                free(temp_moves);
                }
            }
        }
    }

    // вызываем рекурсию
    for (int i = 0; i < size; i++) {
         cur_score = get_best_moves(mas_boards[i], deep-1, color);
         if (max_score < cur_score) {
            max_score = cur_score;
         }
         //printf("free board\n");
         //free(mas_boards[i]);
    }

    // освобождаем выделенную память
    destroy(moves_cur_cat);
    free(moves_cur_cat);
    destroy(mov_on_turn);
    free(mov_on_turn);
    destroy(eat_on_turn);
    free(eat_on_turn);
    free(cur_board);
    return max_score;
}

void get_best_moves_wrapper(Board* board, coordinates_list* list_of_moves,
 coordinates* movable_cat, int deep) {
    // вспомогательные
    char that_turn = board->turn;
    int max_score = -1;
    int arg_max_score;
    int max_scores[] = {-1, -1, -1};

    Board* cur_board = (Board*) malloc(sizeof(Board));

    // изменяемые
    Board* mas_boards[max_size];
    for (int i = 0; i < max_size; i++)
        mas_boards[i] = (Board*) malloc(sizeof(Board));
    coordinates_list* mas_lists_of_moves[max_size];
    for (int i = 0; i < max_size; i++)
        mas_lists_of_moves[i] = (coordinates_list*) malloc(sizeof(coordinates_list));
    coordinates mas_movable_cat[max_size];
    int size = 0;

    // временные
    int cur_score = -1;
    coordinates cur_movable_cat;
    coordinates cur_move;
    coordinates_list* moves_cur_cat = (coordinates_list*) malloc(sizeof(coordinates_list));
    initCL(moves_cur_cat);
    coordinates_list* mov_on_turn = (coordinates_list*) malloc(sizeof(coordinates_list));
    initCL(mov_on_turn);
    coordinates_list* eat_on_turn = (coordinates_list*) malloc(sizeof(coordinates_list));
    initCL(eat_on_turn);

    // шашки, которые могут двигаться
    get_movable_cat(board, mov_on_turn, eat_on_turn);

    // ищем ходы без съедения
    while (mov_on_turn->size > 0) {
        cur_score = -1;
        pop_back(mov_on_turn, &cur_movable_cat);
        get_moves(board, cur_movable_cat, moves_cur_cat);
        while (moves_cur_cat->size > 0) {
            get_copy(board, cur_board);
            pop_back(moves_cur_cat, &cur_move);
            move(cur_board, cur_movable_cat, cur_move);
            change_turn_and_finally_eat(cur_board);
            cur_score = get_score(cur_board);
            if (size < max_size) {
                mas_boards[size] = cur_board;
                max_scores[size] = cur_score;
                mas_movable_cat[size] = cur_movable_cat;
                initCL(mas_lists_of_moves[size]);
                push_back(mas_lists_of_moves[size++], cur_move);
            }
            else {
                for (int i = 0; i < max_size; i++) { // ищем максимум для хода соперника
                    if (cur_score > max_scores[i]) {
                        mas_boards[i] = cur_board;
                        max_scores[i] = cur_score;
                        mas_movable_cat[i] = cur_movable_cat;
                        destroy(mas_lists_of_moves[i]);
                        push_back(mas_lists_of_moves[i], cur_move);
                        break;
                    }
                }
            }
        }
    }

    // ищем ходы со съедением
    while (eat_on_turn->size > 0) {
        cur_score = -1;
        pop_back(eat_on_turn, &cur_movable_cat);
        get_moves(board, cur_movable_cat, moves_cur_cat);
        while (moves_cur_cat->size > 0) {
            coordinates_list* temp_moves = (coordinates_list*) malloc(sizeof(coordinates_list));
            initCL(temp_moves);
            get_copy(board, cur_board);
            pop_back(moves_cur_cat, &cur_move);
            push_back(temp_moves, cur_move);
            move(cur_board, cur_movable_cat, cur_move);
            //используем все возможные ходы
            while (is_able_to_move(cur_board, cur_move) == 1) {
                coordinates temp_cat;
                temp_cat.height = cur_move.height;
                temp_cat.width = cur_move.width;

                coordinates_list* t_moves = (coordinates_list*) malloc(sizeof(coordinates_list));
                initCL(t_moves);
                get_moves(cur_board, temp_cat, t_moves);
                pop_back(t_moves, &cur_move);
                push_back(temp_moves, cur_move);
                move(cur_board, temp_cat, cur_move);
            }
            change_turn_and_finally_eat(cur_board);

            cur_score = get_score(cur_board);
            if (size < max_size) {  // ищем максимум для хода соперника
                mas_boards[size] = cur_board;
                max_scores[size] = cur_score;
                mas_movable_cat[size] = cur_movable_cat;
                mas_lists_of_moves[size] = temp_moves;
            }
            else {
                char check = 1;
                for (int i = 0; i < max_size; i++) {
                    if (cur_score > max_scores[i]) {
                        check = 0;
                        max_scores[i] = cur_score;
                        mas_boards[i] = cur_board;
                        mas_movable_cat[i] = cur_movable_cat;
                        destroy(mas_lists_of_moves[i]);
                        push_back(mas_lists_of_moves[i], cur_move);
                        break;
                    }
                }
                if (check) {
                destroy(temp_moves);
                free(temp_moves);
                }
            }
        }
    }

    if (size == 1) {
        list_of_moves = mas_lists_of_moves[0];
        movable_cat->height = mas_movable_cat[0].height;
        movable_cat->width = mas_movable_cat[0].width;
        free(cur_board);
        return;
    }

    // вызываем рекурсию
    for (int i = 0; i < size; i++) {
         if (deep > 1) {
            cur_score = get_best_moves(mas_boards[i], deep-1, that_turn);
            }
         else
            cur_score = max_scores[i];
         if (cur_score) {
            max_score = cur_score;
            arg_max_score = i;
         }
    }
    coordinates temp;
    destroy(list_of_moves);

    while (mas_lists_of_moves[arg_max_score]->size > 0) {
        pop_front(mas_lists_of_moves[arg_max_score], &temp);
        push_back(list_of_moves, temp);
    }
    for (int i = 0; i < size; i++) {
        destroy(mas_lists_of_moves[i]);
        free(mas_lists_of_moves[i]);
        //free(mas_boards[i]);
    }
    movable_cat->height = mas_movable_cat[arg_max_score].height;
    movable_cat->width = mas_movable_cat[arg_max_score].width;


    // освобождаем выделенную память
    destroy(moves_cur_cat);
    free(moves_cur_cat);
    destroy(mov_on_turn);
    free(mov_on_turn);
    destroy(eat_on_turn);
    free(eat_on_turn);
    free(cur_board);
}

void get_ai_move(Board* board, coordinates_list* moves, coordinates* cur) {
    int recursion_num = 5; // % 2 != 0
    get_best_moves_wrapper(board, moves, cur, recursion_num);
}

int main() {
    Board b;
    initB(&b);
    coordinates_list moves;
    initCL(&moves);
    coordinates cat;
    printf("hey\n");
    get_ai_move(&b, &moves, &cat);
    printf("cat - %d, %d\nmovs - ", cat.height, cat.width);
    print(&moves);

    return 0;
}

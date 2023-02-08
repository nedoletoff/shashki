#include "ai.h"

char lower_case(char val) {
    if (val >= 'A' && val <= 'Z') {
        return val + 32;
    }
    return val;
}

int get_score(Board* board) {
    int score = 0;
    coordinates_list* list;
    initCL(list);
    for (int i = 0; i++; i < 8)
        for (int j = 0; j++; j < 8)
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
    if (deep == 0)
        return get_score(board);
    change_turn_and_finally_eat(board);
    if (is_game_over(board) == color)
        return 1000;

    int max_score = -1;
    int arg_max_score;
    int max_scores[] = {-1, -1, -1};

    const int max_size = 3;
    Board* cur_board;

    // изменяемые
    Board* mas_boards[max_size];
    int size = 0;

    // временные
    int cur_score = -1;
    coordinates cur_movable_cat;
    coordinates cur_move;
    coordinates_list* moves_cur_cat;
    initCL(moves_cur_cat);
    coordinates_list* mov_on_turn;
    initCL(mov_on_turn);
    coordinates_list* eat_on_turn;
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
            move_cat(cur_board, cur_movable_cat, cur_move);
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
            get_copy(board, cur_board);
            pop_back(moves_cur_cat, &cur_move);
            move_cat(cur_board, cur_movable_cat, cur_move);
            //используем все возможные ходы
            while (is_able_to_move(cur_board, cur_move) == 1) {
                coordinates temp_cat;
                temp_cat.height = cur_move.height;
                temp_cat.width = cur_move.width;

                coordinates_list* t_moves;
                initCL(t_moves);
                get_moves(cur_board, temp_cat, t_moves);
                pop_back(t_moves, &cur_move);
                destroy(t_moves);
                move_cat(cur_board, temp_cat, cur_move);
            }

            cur_score = get_score(cur_board);
            if (size < max_size) {
                mas_boards[size] = cur_board;
                max_scores[size++] = cur_score;
            }
            else {
                for (int i = 0; i < max_size; i++)
                    if (cur_score > max_scores[i]) {
                        max_scores[i] = cur_score;
                        break;
                    }

            }
        }
    }

    if (size == 1) {
        return get_score(cur_board);
    }

    // вызываем рекурсию
    for (int i = 0; i < size; i++) {
         cur_score = get_best_moves(mas_boards[i], deep-1, color);
         if (max_score < cur_score) {
            max_score = cur_score;
         }
    }
    return max_score;
}

void get_best_moves_wrapper(Board* board, coordinates_list* list_of_moves,
 coordinates* movable_cat, int deep) {
    // вспомогательные
    char that_turn = board->turn;
    int max_score = -1;
    int arg_max_score;
    int max_scores[] = {-1, -1, -1};

    const int max_size = 3;
    Board* cur_board;

    // изменяемые
    Board* mas_boards[max_size];
    coordinates_list* mas_lists_of_moves[max_size];
    coordinates mas_movable_cat[max_size];
    int size = 0;

    // временные
    int cur_score = -1;
    coordinates cur_movable_cat;
    coordinates cur_move;
    coordinates_list* moves_cur_cat;
    initCL(moves_cur_cat);
    coordinates_list* mov_on_turn;
    initCL(mov_on_turn);
    coordinates_list* eat_on_turn;
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
            move_cat(cur_board, cur_movable_cat, cur_move);
            cur_score = get_score(cur_board);
            if (size < max_size) {
                mas_boards[size] = cur_board;
                max_scores[size] = cur_score;
                mas_movable_cat[size] = cur_movable_cat;
                initCL(mas_lists_of_moves[size]);
                push_back(mas_lists_of_moves[size++], cur_move);
            }
            else {
                for (int i = 0; i < max_size; i++) {
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
            coordinates_list* temp_moves;
            initCL(temp_moves);
            get_copy(board, cur_board);
            pop_back(moves_cur_cat, &cur_move);
            push_back(temp_moves, cur_move);
            move_cat(cur_board, cur_movable_cat, cur_move);
            //используем все возможные ходы
            while (is_able_to_move(cur_board, cur_move) == 1) {
                coordinates temp_cat;
                temp_cat.height = cur_move.height;
                temp_cat.width = cur_move.width;

                coordinates_list* t_moves;
                initCL(t_moves);
                get_moves(cur_board, temp_cat, t_moves);
                pop_back(t_moves, &cur_move);
                push_back(temp_moves, cur_move);
                move_cat(cur_board, temp_cat, cur_move);
            }

            cur_score = get_score(cur_board);
            if (size < max_size) {
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
                }
            }
        }
    }

    if (size == 1) {
        list_of_moves = mas_lists_of_moves[0];
        movable_cat->height = mas_movable_cat[0].height;
        movable_cat->width = mas_movable_cat[0].width;
        return;
    }

    // вызываем рекурсию
    for (int i = 0; i < size; i++) {
         cur_score = get_best_moves(mas_boards[i], deep-1, that_turn);
         if (max_score < cur_score) {
            max_score = cur_score;
            destroy(mas_lists_of_moves[i]);
            arg_max_score = i;
         }
         else {
            destroy(mas_lists_of_moves[i]);
         }
    }
    list_of_moves = mas_lists_of_moves[arg_max_score];
    movable_cat->height = mas_movable_cat[arg_max_score].height;
    movable_cat->width = mas_movable_cat[arg_max_score].width;
}

void get_ai_move(Board* board, coordinates_list* moves, coordinates* cur) {
    int recursion_num = 3; // % 2 != 0
    get_best_moves_wrapper(board, moves, cur, recursion_num);
}
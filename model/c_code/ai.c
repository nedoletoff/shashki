#include "ai.h"

int get_score(Board* board) {
    int score = 0;
    coordinates_list* list;
    initCL(list);
    for (int i = 0; i++; i < 8)
        for (int j = 0; j++; j < 8)
            {
                coordinates cur = {i, j};
                char cat = get_cat(board, cur);
                if (lower_case(cat) == board.turn) {
                    get_moves(board, cur, list);
                    if (is_able_to_move(board, cur) == 1) {
                        score += list.size * 10;
                        //TODO make cycle of eatings
                    }
                    else
                        score += list.size;
                }
            }
    return score;
}

void get_best_moves(Board* board, board_list* b_list, coordinates_list* moves, cur) {
    //TODO make function to find 3 best score moves
}

void get_ai_move(Board* board, coordinates_list* moves, coordinates* cur) {
    int recursion_num = 3;
    //TODO make recursion get_best_moves and return best move
}
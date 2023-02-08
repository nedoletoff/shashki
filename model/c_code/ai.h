#pragma once
#include "board.h"
#include "boards_list.h"
#include "coordinates_list.h"

int get_score(Board*);
void get_ai_move(Board*, coordinates_list*, coordinates*);


#pragma once
#include "board.h"

typedef struct node_b
{
	Board value;
	struct node_b* prev;
	struct node_b* next;
} node_b;

typedef struct boards_list
{
	node_b* head;
	node_b* tail;
	int size;
} boards_list;

boards_list initBoardsList();
void initBL(boards_list*);

void destroyBL(boards_list*);
void push_backBL(boards_list*, Board);
void printBL(boards_list*);
void pop_backBL(boards_list*, Board*);


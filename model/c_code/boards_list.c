#include <stdio.h>
#include <stdlib.h>
#include "boards_list.h"


void initBL(boards_list* l)		//O(1)
{
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
}

boards_list initBoardsList()
{
    boards_list res;
    res.head = NULL;
    res.tail = NULL;
    res.size = 0;
    return res;
}

void destroyBL(boards_list* l)		//O(n)
{
	Board res;
	while (l->size > 0)
	{
        pop_backBL(l, &res);
	}
}

void push_backBL(boards_list* l, Board* val)	//O(1)
{
    node_b* n;
	node_b* cur;
	n = (node_b*) malloc(sizeof(node_b));
	printf("Damn\n");
    print_board(val);

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            n->value.grid[i][j] = val->grid[i][j];

    n->value.white_num = val->white_num;
    n->value.grey_num = val->grey_num;
    n->value.turn = val->turn;
    n->value.eaten = val->eaten;
    n->value.no_eat_counter = val->no_eat_counter;
    n->value.has_moves = val->has_moves;

    n->next = NULL;
	if (l->head == NULL)
	{
		n->prev = NULL;
		l->head = n;
	}
	else
	{
		cur = l->tail;
		cur->next = n;
		n->prev = cur;
	}
	l->tail = n;
	++l->size;
}

void printBL(boards_list* l)		//O(n)
{
	node_b* cur = l->head;
	while (cur != NULL)
	{
	    print_board(&cur->value);
		cur = cur->next;
	}
	printf("\nsize - %d\n", l->size);

}

void pop_backBL(boards_list* l, Board* res)
{
  if (l->size == 0)
    return;
  node_b* cur = l->tail;
  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 8; ++j) {
        res->grid[i][j] = cur->value.grid[i][j];
    }
  res->white_num = cur->value.white_num;
  res->grey_num = cur->value.grey_num;
  res->turn = cur->value.turn;
  res->eaten = cur->value.eaten;
  res->no_eat_counter = cur->value.no_eat_counter;
  res->has_moves = cur->value.has_moves;

  l->tail = cur->prev;
  if (l->tail != NULL)
    l->tail->next = NULL;
  else
    l->head = NULL;
  free(cur);
  --l->size;
}



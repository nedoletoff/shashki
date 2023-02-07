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

    get_copy(val, &n->value);

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

  get_copy(&cur->value, res);

  l->tail = cur->prev;
  if (l->tail != NULL)
    l->tail->next = NULL;
  else
    l->head = NULL;
  free(cur);
  --l->size;
}



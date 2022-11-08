#include <stdio.h>
#include <stdlib.h>
#include "coordinates_list.h"

void init(coordinates_list* l)		//O(1)
{
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
}

void destroy(coordinates_list* l)		//O(n)
{
	node_t* cur = l->head;
	node_t* perv = NULL;
	while (cur != NULL)
	{
		perv = cur;
		cur = cur->next;
		free(perv);
	}
}

void push_back(coordinates_list* l, coordinates val)	//O(1)
{
	node_t* n;
	node_t* cur;
	n = (node_t*) malloc(sizeof(node_t));
	n->value = val; n->next = NULL;
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

void print(coordinates_list* l)		//O(n)
{
	node_t*cur = l->head;
	while (cur != NULL)
	{
		printf("height - %d, width - %d ", cur->value.height, cur->value.width);
		cur = cur->next;
	}
	printf("\n");
}

int set(coordinates_list* l, int i, coordinates val)	//O(n)
{
	node_t* cur = l->head;
	int count = 0;
	if (i >= 0 && i < l->size)
	{
		while (count++ != i)
			cur = cur->next;
		cur->value = val;
		return 1;
	}
	return 0;
}

int get(coordinates_list* l, int i, coordinates* ans)	//O(n)
{
	node_t* cur = l->head;
	int count = 0;
	if (i >= 0 && i < l->size)
	{
		while (count++ != i)
			cur = cur->next;
		*ans = cur->value;
		return 1;
	}
	return 0;
}

void insert(coordinates_list* l, node_t* cur, coordinates val)	//O(1)
{
	node_t* n = (node_t*) malloc(sizeof(node_t));
	if (cur == l->tail)
		l->tail = n;
	n->value = val;
	n->next = cur-> next;
	n->prev = cur;
	cur->next->prev = n;
	cur->next = n;
	++l->size;
}

void erase(coordinates_list* l, node_t* cur)	//O(1)
{
	if (cur == l->tail)
		l->tail = cur->prev;
	if (cur == l->head)
	{
		l->head = cur->next;
		if (cur->next != NULL) cur->next->prev = NULL;
		free(cur);
	}
	else
	{
		cur->prev->next = cur->next;
		if (cur->next != NULL) cur->next->prev = cur->prev;
		free(cur);
	}
	--l->size;
}

void push_front(coordinates_list* l, coordinates val)	//O(1)
{
	node_t* n = (node_t*) malloc(sizeof(node_t));
	if (l->tail == l->head)
		l->tail = n;
	n->value = val;
	n->prev = NULL;
	n->next = l->head;
	l->head->prev = n;
	l->head = n;
	++l->size;
}

void pop_back(coordinates_list* l, coordinates* res)
{
  node_t* cur = l->tail;
  res->height = cur->value.height;
  res->width = cur->value.width;

  l->tail = cur->prev;
  l->tail->next = NULL;
  free(cur);
}



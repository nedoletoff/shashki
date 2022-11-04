#pragma once

typedef struct coordinates {
  int height;
  int width;
} coordinates;

typedef struct node_t
{
	coordinates value;
	struct node_t* prev;
	struct node_t* next;
} node_t;

typedef struct
{
	node_t* head;
	node_t* tail;
	int size;
} list_t;

void init(list_t*);
void destroy(list_t*);
void push_back(list_t*, coordinates);
void print(list_t*);
int set(list_t*, int, coordinates);
int get(list_t*, int, coordinates*);
void erase(list_t*, node_t*);
void pop_back(list_t*, coordinates*);


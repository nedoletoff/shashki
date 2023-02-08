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

typedef struct coordinates_list
{
	node_t* head;
	node_t* tail;
	int size;
} coordinates_list;

coordinates_list initCoordinatesList();
void initCL(coordinates_list*);

void destroy(coordinates_list*);
void push_back(coordinates_list*, coordinates);
void print(coordinates_list*);
int set(coordinates_list*, int, coordinates);
int get(coordinates_list*, int, coordinates*);
void erase(coordinates_list*, node_t*);
void pop_back(coordinates_list*, coordinates*);
void pop_front(coordinates_list*, coordinates*);


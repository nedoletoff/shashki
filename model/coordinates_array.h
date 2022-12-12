#pragma once

typedef struct coordinates {
  int height;
  int width;
} coordinates;

typedef struct coordinates_array
{
	int heights[16];
	int widths[16];
	int size;
} coordinates_array;

coordinates_array initCoordinatesArray();
void init(coordinates_array*);
void destroy(coordinates_array*);
void push_back(coordinates_array*, coordinates);
void print(coordinates_array*);
void pop_back(coordinates_array*, coordinates*);

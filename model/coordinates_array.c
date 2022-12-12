#include <stdio.h>
#include "coordinates_array.h"


coordinates_array initCoordinatesArray() {
    coordinates_array arr;
    arr.size = 0;
    for (int i = 0; i < 16; i++) {
        arr.heights[i] = -1;
        arr.widths[i] = -1;
    }
    return arr;
}
void init(coordinates_array* arr) {
    arr->size = 0;
}
void destroy(coordinates_array* arr) {
    arr->size = 0;
}
void push_back(coordinates_array* arr, coordinates cords) {
    if (arr->size + 1 >= 16) {
        printf("Array size ended\n");
        return;
    }
    arr->heights[arr->size] = cords.height;
    arr->widths[arr->size++] = cords.width;
}
void print(coordinates_array* arr) {
    for (int i = 0; i < arr->size; i++)
		printf("height - %d, width - %d,\t", arr->heights[i], arr->widths[i]);

	printf("\nsize - %d\n", arr->size);
}
void pop_back(coordinates_array* arr, coordinates* cords) {
    if (arr->size <= 0)
        printf("Array ended\n");
    cords->height = arr->heights[--arr->size];
    cords->width = arr->widths[arr->size];
}

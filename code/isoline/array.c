#include <stdio.h>
#include <stdlib.h>
#include "array.h"

/* Initialized an array with maxSize = size. */
void initArray(Array *array, int size) {
	array->length = 0;
	array->maxSize = size;
	array->data = calloc(size, sizeof(int));
}

/* Doubles the maximum size of the array. */
void doubleSize(Array *array) {
	int newSize = array->maxSize * 2;
	array->data  = realloc(array->data, newSize*sizeof(int));	
	array->maxSize = newSize;
}

/* Adds an element to the array. If the array is full, its size is doubled. */
void addElement(Array *array, int element) {
	if(array->length == array->maxSize) {
		doubleSize(array);
	}
	array->data[array->length] = element;
	array->length++;
}

void printArray(Array array) {
	for(int i = 0; i < array.length; i++) {
		fprintf(stdout, "%d ", array.data[i], i);
	}
	fprintf(stdout, "\n");
}

/* Free Array's memory. */
void freeArray(Array array) {
	free(array.data);
}

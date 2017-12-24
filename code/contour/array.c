#include <stdio.h>
#include <stdlib.h>
#include "array.h"

/* Initialized an array with maxSize = size. */
void initArray(Array *array, int size) {
	array->length = 0;
	array->maxSize = size;
	array->data = calloc(size, sizeof(double));
}

/* Doubles the maximum size of the array. */
void doubleSize(Array *array) {
	int newSize = array->maxSize * 2;
	array->data  = realloc(array->data, newSize*sizeof(double));	
	array->maxSize = newSize;
}

/* Adds an element to the array. If the array is full, its size is doubled. */
void addElement(Array *array, double element) {
	if(array->length == array->maxSize) {
		doubleSize(array);
	}
	array->data[array->length] = element;
	array->length++;
}

void printArray(Array array) {
	fprintf(stdout, "Array data:\n[");
	for(int i = 0; i < array.length - 1; i++) {
		fprintf(stdout, "%.2f, ", array.data[i]);
	}
	fprintf(stdout, "%.2f]\n\n", array.data[array.length - 1]);
}

double getArraySum(Array array) {
	double sum = 0;
	for(int i = 0; i < array.length; i++) sum += array.data[i];
	return sum;
}

/* Free Array's memory. */
void freeArray(Array array) {
	free(array.data);
}

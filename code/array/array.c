#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

/* Initialized an array with maxSize = size. */
void initArray(Array *array) {
	array->length = 0;
	array->maxSize = 1;
	array->data = calloc(1, sizeof(double));
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

/* Creates a copy of the array. */
Array copyArray(Array x) {
	Array copy; 
	initArray(&copy);
	for(int i=0; i < x.length; i++) {
		addElement(&copy, x.data[i]);
	}
	return copy;
}

void popElement(Array *x) {
	x->length--;
	x->data[x->length] = 0;
}

/* Print the array on the standard output. */
void printArray(Array array) {
	fprintf(stdout, "[");
	for(int i = 0; i < array.length - 1; i++) {
		fprintf(stdout, "%.2f, ", array.data[i]);
	}
	fprintf(stdout, "%.2f]\n\n", array.data[array.length - 1]);
}

/* Return the sum of the array. */
double getArraySum(Array array) {
	double sum = 0;
	for(int i = 0; i < array.length; i++) sum += array.data[i];
	return sum;
}

/* Free Array's memory. */
void freeArray(Array array) {
	free(array.data);
}

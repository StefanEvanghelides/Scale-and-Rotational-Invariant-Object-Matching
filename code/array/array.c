#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/* Creates a copy of the array. */
Array copyArray(Array x) {
	Array copy; 
	initArray(&copy, x.length);
	for(int i=0; i < x.length; i++) {
		addElement(&copy, x.data[i]);
	}
	return copy;
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

/* Stretch array x to have the same size as the base array. */
void stretchArray(Array *base, Array *x) {
	// Initialize new array
	Array stretched; initArray(&stretched, base->length);

	// Add new values:
	// for Double step = len(base) / len(x), make sure that every time step reaches 1,
	//      add an interpolated value into the array
	double baseStep = (double) base->length / x->length - 1;
	double currentStep = baseStep;
	for(int idx=0; idx < x->length; idx++) {
		while(currentStep >= 1) {
			// Add the interpolated value
			if(idx > 0) addElement(&stretched, (x->data[idx-1] + x->data[idx]) / 2);
			else addElement(&stretched, (x->data[idx] + x->data[x->length - 1]) / 2);	

			currentStep -= 1; // Set currentStep with 1 value lower
		}
		addElement(&stretched, x->data[idx]);
		currentStep += baseStep;
	}

	// Make x = stretched
	for(int i = x->length; i<base->length; i++) {
		addElement(x, 0); //add dummy values to match the length
	}
	for(int i=0; i < x->length; i++) {
		x->data[i] = stretched.data[i];
	}
}

/* Free Array's memory. */
void freeArray(Array array) {
	free(array.data);
}

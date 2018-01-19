#ifndef ARRAY_H
#define ARRAY_H

/* Structure representing the Array. */
typedef struct {
	int length;
	int maxSize;
	double *data;
} Array;

/* Prototypes. */
void initArray(Array *array, int size);
void doubleSize(Array *array);
void addElement(Array *array, double element);
void printArray(Array array);
void freeArray(Array array);
double getArraySum(Array array);
void stretchArray(Array *base, Array *x);

#endif
 

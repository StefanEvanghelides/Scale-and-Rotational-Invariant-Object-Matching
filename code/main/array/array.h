#ifndef ARRAY_H
#define ARRAY_H

/* Structure representing the Array. */
typedef struct {
	int length;
	int maxSize;
	double *data;
} Array;

/* Prototypes. */
void initArray(Array *array);
void doubleSize(Array *array);
void addElement(Array *array, double element);
Array copyArray(Array x);
void popElement(Array *x);
void printArray(Array array);
double getArraySum(Array array);
void freeArray(Array array);

#endif
 

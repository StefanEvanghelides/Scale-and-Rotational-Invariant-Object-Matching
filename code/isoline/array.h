#ifndef ARRAY_H
#define ARRAY_H

/* Structure representing the Array. */
typedef struct {
	int length;
	int maxSize;
	int *data;
} Array;

/* Prototypes. */
void initArray(Array *array, int size);
void doubleSize(Array *array);
void addElement(Array *array, int element);
void printArray(Array array);
void freeArray(Array array);

#endif
 

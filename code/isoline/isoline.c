#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../pgm_io/pgm_io.h"
#include "isoline.h"

int getAngle(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold) {
	return 0;
}

bool isEdge(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold) {
	return ((topLeft<threshold || topRight<threshold || bottomLeft<threshold || bottomRight<threshold)
		&& !(topLeft>threshold && topRight>threshold && bottomLeft>threshold && bottomRight>threshold)
		&& !(topLeft<threshold && topRight<threshold && bottomLeft<threshold && bottomRight<threshold));
}


/* This function will create the contour of an object,
   which will be represented as an 1D array of angles.
   These angles are based on the relative position of the threshold,
   applied to a group of 2X2 pixels.
   (see Marching Squares algorithm)  */
int *createContour(PGMImage image, int threshold) {
	int row,col;
	int topLeft, topRight, bottomLeft, bottomRight;
	int *angles = NULL;
	
	int count = 0;
	for(row = 0; row < image.height - 1; row++) {
		for(col = 0; col < image.width - 1; col++) {
			topLeft = image.data[row][col];         
			topRight = image.data[row][col+1];      
			bottomLeft = image.data[row+1][col];    
			bottomRight = image.data[row+1][col+1]; 

			if(isEdge(topLeft, topRight, bottomLeft, bottomRight, threshold)) {
				count++;
				fprintf(stdout, "Edge on top left position (%d,%d)\n", row, col);
				int angle = getAngle(topLeft, topRight, bottomLeft, bottomRight, threshold);
			}

		}
	}
	fprintf(stdout, "Count = %d\n", count);
	
	return angles;
}
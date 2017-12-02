#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../pgm_io/pgm_io.h"
#include "isoline.h"

/* This function will convert a grayscale PGM image
   into a b/w picture based on a certain threshold. */
PGMImage applyThreshold(PGMImage image, int threshold) {
	int row, col;
	PGMImage binaryImage = initializePGMImage(image.type, image.width, image.height, 1);
	
	for(row = 0; row < binaryImage.height; row++) {
		for(col = 0; col < binaryImage.width; col++) {
			if(image.data[row][col] < threshold) {
				binaryImage.data[row][col] = 0;
			} else {
				binaryImage.data[row][col] = 1;
			}
		}
	}
	
	return binaryImage;
}

/* Returns the contour case of a group of 4 pixels (2X2). Their value is as follows: 
   top left = 8, top right = 4, bottom left = 1, bottom right = 2. 
   So, they are powers of 2, decreasing in clock-wise motion. 
   NOTE: they is expected to be greater than 0.*/
int getContourCase(int topLeft, int topRight, int bottomLeft, int bottomRight) {
	int contourCase = 0;
	
	if(topLeft > 0)     contourCase += 8;
	if(topRight > 0)    contourCase += 4;
	if(bottomLeft > 0)  contourCase += 1;
	if(bottomRight > 0) contourCase += 2;
	
	return contourCase;
}

/* This function will create the contour of an object.
   This will result in a black and white PGM image,
   in which the contour is black and the background is white.  */
PGMImage createContour(PGMImage image) {
	PGMImage contour = initializePGMImage(image.type, image.width-1, image.height-1, 15);
	int row,col;
	int topLeft, topRight, bottomLeft, bottomRight;
	int contourCase; /* This is a value between 0 and 15. */
	
	PGMImage binaryImage = applyThreshold(image, 140);
	
	for(row = 0; row < image.height - 1; row++) {
		for(col = 0; col < image.width - 1; col++) {
			topLeft = binaryImage.data[row][col];         
			topRight = binaryImage.data[row][col+1];      
			bottomLeft = binaryImage.data[row+1][col];    
			bottomRight = binaryImage.data[row+1][col+1]; 
			
			contourCase = getContourCase(topLeft, topRight, bottomLeft, bottomRight);
			contour.data[row][col] = contourCase;
		}
	}
	
	return contour;
}
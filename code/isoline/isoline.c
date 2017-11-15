#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../pgm_io/pgm_io.h"
#include "isoline.h"

/* This function will convert a grayscale PGM image
   into a b/w picture based on a certain threshold. */
PGMImage applyThreshold(PGMImage image, int threshold) {
	int row, col;
	PGMImage binaryImage = initializePGMImage(image.width, image.height, 1);
	
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

/* This function will create the contour of an object.
   This will result in a black and white PGM image,
   in which the contour is black and the background is white.  */
PGMImage createContour(PGMImage image) {
	PGMImage contour = initializePGMImage(image.width, image.height, 15);
	int row,col;
	
	PGMImage binaryImage = applyThreshold(image, 128);
	
	for(row = 0; row < image.height - 1; row++) {
		for(col = 0; col < image.width - 1; col++) {
			
		}
	}
	
	return contour;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "isoline.h"
#include "./pgm_io/pgm_io.h"

/* This function will convert a grayscale PGM image
   into a b/w picture based on a certain threshold. */
PGMImage applyThreshold(PGMImage img, int threshold) {
	PGMImage binaryImage;
	
	
	return binaryImage;
}

/* This function will create the contour of an object.
   This will result in a black and white PGM image,
   in which the contour is black and the background is white.  */
PMGImage createContour(PGMImage img) {
	PGMImage contour;
	int row,col;
	
	
	for(row = 0; row < img->height - 1; row++) {
		for(col = 0; col < img->width - 1; col++) {
			
		}
	}
	
	return contour;
}
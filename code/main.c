#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm_io/pgm_io.h"
#include "isoline/isoline.h"

/* This function creates the negative of a grayscale PGM image. */
void negative(PGMImage *image) {
	int row, col;
	unsigned char ch;
	for(row=0; row<image->height; row++) {
		for(col=0; col<image->width; col++) {
			image->data[row][col] = (image->data[row][col] + 127) % 256;
		}
	}
}

int main(int argc, char** argv) {
	
	PGMImage image = readPGM("mask.pgm");
	//PGMImage contour = applyThreshold(image, 140);
	PGMImage contour = createContour(image);
	writePGM("mask_contour.pgm", contour);
	
	return 0;
}
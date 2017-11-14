#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm_io/pgm_io.h"

/* This function creates the negative of a grayscale PGM image. */
void negative(PGMImage *img) {
	int row, col;
	unsigned char ch;
	for(row=0; row<img->height; row++) {
		for(col=0; col<img->width; col++) {
			ch = (img->data[row][col] + 127) % 256;
			img->data[row][col] = ch;
		}
	}
}

int main(int argc, char** argv) {
	PGMImage image;
	
	readPGM("mask.pgm", &image);
	
	writePGM("mask_saved.pgm", &image);
	
	return 0;
}
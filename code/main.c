#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pgm/pgm.h"
#include "isoline/array.h"
#include "isoline/isoline.h"

void printImage(PGMImage image) {
	int row, col;
	for(row = 0; row < image.height; row++) {
		for(col = 0; col < image.width; col++) {
			fprintf(stdout, "%d ", image.data[row][col]);
		}
		fprintf(stdout, "\n");
	}
}

int main(int argc, char** argv) {
	
	/* Read the PGM Image. */
	PGMImage image = readPGM("mask.pgm");

	/* Print the values on the standard output. */
	printImage(image);

	/* For now, the threshold will be 142. */
	Array angles = createContour(image, 142);

	/* Print array. */
	printArray(angles);

	/* Free memory. */
	freeArray(angles);
	freePGM(image);

	return 0;
}
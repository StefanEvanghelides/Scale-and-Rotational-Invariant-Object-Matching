#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pgm/pgm.h"
#include "contour/array.h"
#include "contour/contour.h"

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
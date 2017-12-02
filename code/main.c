#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pgm_io/pgm_io.h"
#include "isoline/isoline.h"

int main(int argc, char** argv) {
	
	/* Read the PGM Image. */
	PGMImage image = readPGM("mask.pgm");

	/* For now, the threshold will be 140. */
	int* angles = createContour(image, 140);

	/* Print array. */
	int i = 0;
	fprintf(stdout, "Printing Angles...\n");
	while(angles+i != NULL) {
		fprintf(stdout, "%d ", angles[i]);
	}
	
	return 0;
}
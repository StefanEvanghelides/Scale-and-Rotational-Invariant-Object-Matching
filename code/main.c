#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pgm/pgm.h"
#include "contour/array.h"
#include "contour/contour.h"

int main(int argc, char** argv) {
	/* Check argumetns. */
	if(argc < 2) {
		fprintf(stderr, "ERROR: Missing the name of the file!\nUsage: ./RUN <file_name>\n\n");
		exit(-1);
	}

	/* Read the PGM Image. */
	PGMImage image = readPGM(argv[1]);

	/* Print the values on the standard output. */
	printImage(image);

	/* For now, the threshold will be 142. */
	Array angles = createContour(image, 142);

	/* Print array. */
	printArray(angles);

	/* Print the sum. */
	fprintf(stdout, "Sum Angles = %.2f\n\n", getArraySum(angles));

	/* Free memory. */
	freeArray(angles);
	freePGM(image);

	return 0;
}
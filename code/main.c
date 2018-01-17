#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pgm/pgm.h"
#include "contour/array.h"
#include "contour/contour.h"
#include "correlation/correlation.h"


Array flattenImage(char *filename, char *thresholdString) {
	int threshold = (int) strtol(thresholdString, NULL, 10);
	if(threshold < 1 || threshold > 255) {
		fprintf(stderr, "Invalid Threshold! Threshold = %d\n\n", threshold);
		exit(-1);
	}

	PGMImage image = readPGM(filename);
	Array contour = createContour(image, threshold);
	freePGM(image);

	return contour;
}

int main(int argc, char** argv) {
	Array anglesF1T1, anglesF2T2;

	/* Check arguments. If they fail, exit the program with -1 code.*/
	if(argc != 3 &&  argc != 5) {
		fprintf(stderr, "ERROR: Could not run the program!\n\n"
			            "Usage: ./RUN <file_name F1> <Threshold T1 for F1> "
			            "[<file_name F2> <Threshold T2 for F2>]\n\n"
			            "Enter 1 filename with its threshold for computing only the array of angles "
			            "OR enter 2 filenames with their respective thresholds to also compute the correlation\n\n");
		exit(-1);
	}

	/* Checks the arguments for the first file and threshold. */
	if(argc >= 3) {
		anglesF1T1 = flattenImage(argv[1], argv[2]);
		fprintf(stdout, "%s:\n", argv[1]); printArray(anglesF1T1);

		/* Check the arguments for the second file and threshold (optional). */
		if(argc == 5) {
			anglesF2T2 = flattenImage(argv[3], argv[4]);
			fprintf(stdout, "%s:\n", argv[3]); printArray(anglesF2T2);

			/* Correlation with Pearson Correlator. */
			double corr = correlation(anglesF1T1, anglesF2T2);
			fprintf(stdout, "Correlation: %lf\n", corr);
		}

		freeArray(anglesF1T1);
		freeArray(anglesF2T2);
	}

	return 0;
}
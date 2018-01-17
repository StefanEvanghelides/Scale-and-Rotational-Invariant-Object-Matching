#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pgm/pgm.h"
#include "contour/array.h"
#include "contour/contour.h"


int main(int argc, char** argv) {
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
		int t1 = (int) strtol(argv[2], NULL, 10);
		if(t1 < 1 || t1 > 255) {
			fprintf(stderr, "Invalid Threshold! Threshold T1 = %d\n\n", t1);
			exit(-1);
		}

		PGMImage image = readPGM(argv[1]);
		Array angles = createContour(image, t1);
		fprintf(stdout, "%s:\n", argv[1]); printArray(angles);

		freeArray(angles);
		freePGM(image);

		/* Check the arguments for the second file and threshold (optional). */
		if(argc == 5) {
			
			int t2 = (int) strtol(argv[4], NULL, 10);
			if(t2 < 1 || t2 > 255) {
				fprintf(stderr, "Invalid Threshold! Threshold T2 = %d\n\n", t2);
				exit(-1);
			}
			PGMImage image = readPGM(argv[3]);
			Array angles = createContour(image, t2);
			fprintf(stdout, "%s:\n", argv[3]); printArray(angles);

			freeArray(angles);
			freePGM(image);

			/* Correlation with Pearson Correlator. */
			fprintf(stdout, "Can correlate now!\n");
		}
	}

	return 0;
}
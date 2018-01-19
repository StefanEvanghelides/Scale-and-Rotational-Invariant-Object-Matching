#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pgm/pgm.h"
#include "array/array.h"
#include "contour/contour.h"
#include "correlation/correlation.h"

char* intToStr(int x) {
	char *str = malloc(4); // expect 3 digits and the terminating char;
	int digit, idx = 0;
	while(x != 0) {
		digit = x % 10;
		x /= 10;
		for(int i=idx; i > 0; i--) {
			str[i] = str[i-1];
		}
		str[0] = digit+'0';
		idx++;
	}
	return str;
}

/* Flattens a 2D image into a 1D array of angles, by storing the angles of the edges,
 * based on the Marching Squares algorithm. */
Array flattenImage(char *filename, char *thresholdString) {
	int threshold = (int) strtol(thresholdString, NULL, 10);
	if(threshold < 1 || threshold > 255) {
		fprintf(stderr, "Invalid Threshold! Threshold = %d\n\n", threshold);
		exit(-1);
	} else fprintf(stdout, "Threshold = %d\n", threshold);

	PGMImage image = readPGM(filename);
	Array contour = createContour(image, threshold);
	fprintf(stdout, "%s:\n", filename);  
	printArray(contour);
	
	freePGM(image);

	return contour;
}

double correlateArrays(Array anglesF1T1, Array anglesF2T2, char *F1, char *T1, char *F2, char *T2) {
	double corr = 0.0;

	anglesF2T2 = flattenImage(F2, T2);
	
	/* Ensure arrays have the same length. */
	if(anglesF1T1.length > anglesF2T2.length) stretchArray(&anglesF1T1, &anglesF2T2);
	else if(anglesF1T1.length < anglesF2T2.length) stretchArray(&anglesF2T2, &anglesF1T1);

	/* Correlation with Pearson Correlator. */
	if(anglesF1T1.length == anglesF2T2.length) {
		fprintf(stdout, "Arrays have the same length = %d\n", anglesF1T1.length);
		corr = correlation(anglesF1T1, anglesF2T2);
		fprintf(stdout, "Correlation: %lf\n\n", corr);
	} else fprintf(stderr, "The 2 arrays do not have the same length!\n\n");

	freeArray(anglesF2T2);

	return corr;
}

/* Executes the program. */
void execute(int argc, char** argv) {
	Array anglesF1T1, anglesF2T2;

	/* Check arguments. If they fail, exit the program with -1 code.*/
	if(argc < 3 &&  argc > 5) {
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

		/* Check the arguments for the second file and threshold (optional). */
		if(argc == 5) {
			correlateArrays(anglesF1T1, anglesF2T2, argv[1], argv[2], argv[3], argv[4]);
		} else {
			/* there are only 4 arguments, meaning that the last image does not have a threshold by defaul.
			 * It will test for all of them, printing the best match. */
			//fprintf(stderr, "No implementation yet!\n\n");
			double maxCorr = 0.0;
			double threshold = 0.0;
			double currentCorr = 0.0;
			for(int i=1; i < 254; i++) {
				currentCorr = correlateArrays(anglesF1T1, anglesF2T2, argv[1], argv[2], argv[3], intToStr(i));
				if(currentCorr > maxCorr) {
					maxCorr = currentCorr;
					threshold = i;
				}
			}
			fprintf(stdout, "Best Correlation = %lf with threshold = %lf", maxCorr, threshold);
		}

		freeArray(anglesF1T1);
	}
}

int main(int argc, char** argv) {
	execute(argc, argv);

	return 0;
}
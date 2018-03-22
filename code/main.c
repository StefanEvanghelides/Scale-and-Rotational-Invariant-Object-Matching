#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pgm/pgm.h"
#include "array/array.h"
#include "contour/contour.h"
#include "correlation/correlation.h"
#include "interpolation/interpolation.h"

char* intToStr(int x) {
	char *str = calloc(4, sizeof(char)); // expect 3 digits and the terminating char;
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

double correlateArrays(Array anglesF1T1, Array anglesF2T2) {
	double corr = -1.0; 

	Array copy1 = copyArray(anglesF1T1);
	Array copy2 = copyArray(anglesF2T2);

	/* Ensure arrays have the same length. */
	if(copy1.length > copy2.length && copy2.length != 0) interpolate(&copy1, &copy2);
	else if(copy1.length < copy2.length && copy1.length != 0) interpolate(&copy2, &copy1);

	/* Correlation with Pearson Correlator. */
	if(copy1.length == copy2.length) {
		fprintf(stdout, "Arrays have the same length = %d\n", copy1.length);

		printf("First:\n"); printArray(copy1);
		printf("Second:\n"); printArray(copy2);

		corr = correlation(copy1, copy2);
		fprintf(stdout, "\nCorrelation: %lf\n\n", corr);
	} else fprintf(stderr, "The 2 arrays do not have the same length!\n\n");

	freeArray(copy1);
	freeArray(copy2);

	return corr;
}

/* Executes the program. */
void execute(int argc, char** argv) {
	Array anglesF1T1, anglesF2T2;

	/* Check arguments. If they fail, exit the program with -1 code.*/
	if(argc < 3 ||  argc > 5) {
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
			anglesF2T2 = flattenImage(argv[3], argv[4]);
			correlateArrays(anglesF1T1, anglesF2T2);
			//freeArray(anglesF2T2);
		} else if(argc == 4){
			/* It will test for all of them, printing the best match. */
			int threshold = 0;
			double currentCorr = 0.0, maxCorr = 0.0;
			for(int i=100; i < 170; i++) {
				anglesF2T2 = flattenImage(argv[3], intToStr(i));
				currentCorr = correlateArrays(anglesF1T1, anglesF2T2);
				if(currentCorr > maxCorr) {
					maxCorr = currentCorr;
					threshold = i;
				}
				freeArray(anglesF2T2);
			}
			fprintf(stdout, "Best Correlation = %lf with threshold = %d\n\n", maxCorr, threshold);
		}

		freeArray(anglesF1T1);
	}
}

int main(int argc, char** argv) {
	char *filename1, *filename2, *threshold1, *threshold2;
	Array anglesF1T1, anglesF2T2;

	printf("Type in your choice:\n"
		   "  0 - test threshold on image\n"
		   "  1 - correlate 2 images\n"
		   "  2 - search for best threshold value\n"
		   "  3 - count frequency of a letter in a page\n\n"
		   "Answer: ");

	int choice;
	scanf("%d", &choice);

	printf("\n");

	switch(choice) {
		case 0:
			printf("Usage:\n");
			printf("  File Name: ");
			filename1 = calloc(100, sizeof(char)); 
			scanf("%s", filename1);

			printf("  Threshold: ");
			threshold1 = calloc(4, sizeof(char)); 
			scanf("%s", threshold1);

			putchar('\n');
			anglesF1T1 = flattenImage(filename1, threshold1);

			freeArray(anglesF1T1);
			free(filename1);
			free(threshold1);

			break;

		case 1:
			/* Read First Image. */
			printf("Usage:\n");
			printf("  File Name F1: ");
			filename1 = calloc(100, sizeof(char)); 
			scanf("%s", filename1);

			printf("  Threshold T1: ");
			threshold1 = calloc(4, sizeof(char)); 
			scanf("%s", threshold1);

			/* Read Second Image. */
			printf("  File Name F2: ");
			filename2 = calloc(100, sizeof(char)); 
			scanf("%s", filename2);

			printf("  Threshold T2: ");
			threshold2 = calloc(4, sizeof(char)); 
			scanf("%s", threshold2);

			/* Create contour lines. */
			putchar('\n');
			anglesF1T1 = flattenImage(filename1, threshold1);
			anglesF2T2 = flattenImage(filename2, threshold2);

			/* Correlate. */
			correlateArrays(anglesF1T1, anglesF2T2);

			/* Free memory. */
			freeArray(anglesF1T1);
			freeArray(anglesF2T2);
			free(filename1);
			free(filename2);
			free(threshold1);
			free(threshold2);

			break;

		case 2:
			/* Read First Image. */
			printf("Usage:\n");
			printf("  File Name F1: ");
			char *filename1 = calloc(100, sizeof(char)); 
			scanf("%s", filename1);

			printf("  Threshold T1: ");
			char *threshold1 = calloc(4, sizeof(char)); 
			scanf("%s", threshold1);

			/* Read Second Image - only the file name. */
			printf("  File Name F2: ");
			char *filename2 = calloc(100, sizeof(char)); 
			scanf("%s", filename2);

			putchar('\n');
			anglesF1T1 = flattenImage(filename1, threshold1);

			/* It will test all relevant thresholds, printing the best match. */
			int threshold = 0;
			double currentCorr = 0.0, maxCorr = 0.0;
			for(int i=100; i < 170; i++) {
				char *thresholdString = intToStr(i);
				anglesF2T2 = flattenImage(filename2, thresholdString);
				currentCorr = correlateArrays(anglesF1T1, anglesF2T2);
				if(currentCorr > maxCorr) {
					maxCorr = currentCorr;
					threshold = i;
				}

				freeArray(anglesF2T2);
				free(thresholdString);
			}
			fprintf(stdout, "Best Correlation = %lf with threshold = %d\n\n", maxCorr, threshold);

			/* Free memory. */
			freeArray(anglesF1T1);
			free(filename1);
			free(filename2);
			free(threshold1);

			break;

		case 3:

			break;
	}

	


	return 0;
}
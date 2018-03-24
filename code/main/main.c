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
Array flattenImage(PGMImage image, char *thresholdString) {
	int threshold = (int) strtol(thresholdString, NULL, 10);
	if(threshold < 1 || threshold > 255) {
		fprintf(stderr, "Invalid Threshold! Threshold = %d\n\n", threshold);
		exit(-1);
	}

	return createContour(image, threshold);
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

		fprintf(stdout, "First:\n"); printArray(copy1);
		fprintf(stdout, "Second:\n"); printArray(copy2);

		corr = correlation(copy1, copy2);
	} else fprintf(stderr, "The 2 arrays do not have the same length!\n\n");

	freeArray(copy1);
	freeArray(copy2);

	return corr;
}

int getCurrentRow(PGMImage image, int threshold, int prevRow) {
	int currentRow = prevRow - 1;
	int minCount = image.width;

	while(currentRow + 1 < image.height) {
		int count = 0;
		for(int j=0; j < image.width; j++) {
			if(image.data[currentRow+1][j] < threshold) count++;
		}

		if(count < minCount) {
			minCount = count;
			currentRow++;
		} else if(minCount < image.width / 10) break;
	}

	return currentRow;
}

int getNextRow(PGMImage image, int threshold, int currentRow) {
	int nextRow = currentRow;
	int minCount = image.width;

	while(nextRow < image.height) {
		int count = 0;
		for(int j=0; j < image.width; j++) {
			if(image.data[nextRow][j] < threshold) count++;
		}

		if(count < minCount) {
			minCount = count;
			nextRow++;
		} else if(minCount < image.width / 10) break;
	}

	return nextRow;
}

int getCurrentCol(PGMImage image, int threshold, int currentRow, int nextRow, int prevCol) {
	int currentCol = prevCol - 1;
	int minCount = image.height;

	while(currentCol + 1 < image.width) {
		int count = 0;
		for(int i = currentRow; i < nextRow; i++) {
			if(image.data[i][currentCol+1] < threshold) count++;
		}

		if(count < minCount) {
			minCount = count;
			currentCol++;	
		} else if(minCount < image.height / 10) break;		
	}

	return currentCol;
}

int getNextCol(PGMImage image, int threshold, int currentRow, int nextRow, int currentCol) {
	int nextCol = currentCol;
	int minCount = image.height;

	while(nextCol < image.width) {
		int count = 0;
		for(int i = currentRow; i < nextRow; i++) {
			if(image.data[i][nextCol] < threshold) count++;
		}

		if(minCount > count) {
			minCount = count;
			currentCol++;
		} else if(minCount < image.height / 10) break;	
	}

	return nextCol;
}

int main(int argc, char** argv) {
	char *filename, *filename2, *threshold, *threshold2;
	int thresholdInt;
	double correlation;
	Array anglesF1T1, anglesF2T2;
	PGMImage image;

	fprintf(stdout, "Type in your choice:\n"
		   			"  1 - test threshold on image\n"
		   			"  2 - correlate 2 images\n"
		   			"  3 - search for best threshold value\n"
		   			"  4 - count frequency of a letter in a page\n\n"
		   			"  0 - EXIT\n\n"
		   			"Answer: ");

	int choice;
	scanf("%d", &choice);

	printf("\n");

	switch(choice) {
		case 1:
			fprintf(stdout, "Usage:\n");
			fprintf(stdout, "  File Name: ");
			filename = calloc(100, sizeof(char)); 
			scanf("%s", filename);

			fprintf(stdout, "  Threshold: ");
			threshold = calloc(4, sizeof(char)); 
			scanf("%s", threshold);

			putchar('\n');
			image = readPGM(filename);
			anglesF1T1 = flattenImage(image, threshold);
			fprintf(stdout, "%s: \n", filename);
			printArray(anglesF1T1);

			freeArray(anglesF1T1);
			freePGM(image);
			free(filename);
			free(threshold);

			break;

		case 2:
			/* Read First Image. */
			fprintf(stdout, "Usage:\n");
			fprintf(stdout, "  File Name F1: ");
			filename = calloc(100, sizeof(char)); 
			scanf("%s", filename);

			fprintf(stdout, "  Threshold T1: ");
			threshold = calloc(4, sizeof(char)); 
			scanf("%s", threshold);

			/* Read Second Image. */
			fprintf(stdout, "  File Name F2: ");
			filename2 = calloc(100, sizeof(char)); 
			scanf("%s", filename2);

			fprintf(stdout, "  Threshold T2: ");
			threshold2 = calloc(4, sizeof(char)); 
			scanf("%s", threshold2);

			/* Create contour lines. */
			putchar('\n');
			image = readPGM(filename);
			anglesF1T1 = flattenImage(image, threshold);
			freePGM(image);

			image = readPGM(filename2);
			anglesF2T2 = flattenImage(image, threshold2);
			freePGM(image);

			/* Correlate. */
			correlation = correlateArrays(anglesF1T1, anglesF2T2);
			fprintf(stdout, "\nCorrelation: %lf\n\n", correlation);

			/* Free memory. */
			freeArray(anglesF1T1);
			freeArray(anglesF2T2);
			free(filename);
			free(filename2);
			free(threshold);
			free(threshold2);

			break;

		case 3:
			/* Read First Image. */
			fprintf(stdout, "Usage:\n");
			fprintf(stdout, "  File Name F1: ");
			filename = calloc(100, sizeof(char)); 
			scanf("%s", filename);

			fprintf(stdout, "  Threshold T1: ");
			threshold = calloc(4, sizeof(char)); 
			scanf("%s", threshold);

			/* Read Second Image - only the file name. */
			fprintf(stdout, "  File Name F2: ");
			filename2 = calloc(100, sizeof(char)); 
			scanf("%s", filename2);


			putchar('\n');
			image = readPGM(filename);
			anglesF1T1 = flattenImage(image, threshold);
			freePGM(image);

			/* It will test all relevant thresholds, printing the best match. */
			thresholdInt = 0;
			double maxCorr = 0.0;
			image = readPGM(filename2);
			for(int i=100; i < 170; i++) {
				threshold2 = intToStr(i);
				anglesF2T2 = flattenImage(image, threshold2);
				correlation = correlateArrays(anglesF1T1, anglesF2T2);
				if(correlation > maxCorr) {
					maxCorr = correlation;
					thresholdInt = i;
				}

				freeArray(anglesF2T2);
				free(threshold2);
			}
			fprintf(stdout, "Best Correlation = %lf with threshold = %d\n\n", maxCorr, thresholdInt);

			/* Free memory. */
			freeArray(anglesF1T1);
			freePGM(image);
			free(filename);
			free(filename2);
			free(threshold);

			break;

		case 4:
			/* Read Image. */
			fprintf(stdout, "Usage:\n");
			fprintf(stdout, "  File Name: ");
			filename = calloc(100, sizeof(char)); 
			scanf("%s", filename);

			fprintf(stdout, "  Threshold: ");
			threshold= calloc(4, sizeof(char)); 
			scanf("%s", threshold);

			/* Read Page. */
			fprintf(stdout, "  Page Name: ");
			filename2 = calloc(100, sizeof(char)); 
			scanf("%s", filename2);

			fprintf(stdout, "  Threshold: ");
			threshold2 = calloc(4, sizeof(char)); 
			scanf("%s", threshold2);

			/* Set the correlation level. */
			fprintf(stdout, "  Correlation: ");
			scanf("%lf", &correlation);




			/* Create contour line of the image. */
			putchar('\n');
			image = readPGM(filename);
			anglesF1T1 = flattenImage(image, threshold);
			freePGM(image);

			/* Split page in multiple components. */
			image = readPGM(filename2);
			int prevRow=0, prevCol=0, currentRow, currentCol, nextRow, nextCol;
			thresholdInt = atoi(threshold2);
			int count = 0;

			while(prevRow < image.height) {
				/* Get next row. */
				currentRow = getCurrentRow(image, thresholdInt, prevRow);
				nextRow = getNextRow(image, thresholdInt, currentRow);

				printf("currentRow = %d, nextRow = %d\n", currentRow, nextRow);

				while(prevCol < image.width) {
					/* Get next column. */
					currentCol = getCurrentCol(image, thresholdInt, currentRow, nextRow, prevCol);
					nextCol = getNextCol(image, thresholdInt, currentRow, nextRow, currentCol);

					printf("currentCol = %d, nextCol = %d\n", currentCol, nextCol);


					/* Extract letter from page. */
					PGMImage subImage = extractSubImage(image, currentRow, currentCol, nextRow, nextCol);

					/* Create contour of the extracted letter. */
					anglesF2T2 = flattenImage(subImage, threshold2);
					/* Correlate. */
					
					double corr = correlateArrays(anglesF1T1, anglesF2T2);
					fprintf(stdout, "Correlation = %lf\n", corr);

					count++;

					prevCol = nextCol;

					/* Free temp memory. */
					freeArray(anglesF2T2);
					freePGM(subImage);

					break;
				}

				/* Current row becomes the new one. */
				prevRow = nextRow;
			}

			printf("Count = %d\n", count);

			/* Free memory. */
			freeArray(anglesF1T1);
			freePGM(image);
			free(filename);
			free(filename2);
			free(threshold);
			free(threshold2);


			break;

		default: break; /* Case 0 - Simply exit. */
	}

	return 0;
}
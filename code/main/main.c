#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pgm/pgm.h"
#include "array/array.h"
#include "contour/contour.h"
#include "correlation/correlation.h"
#include "interpolation/interpolation.h"

/* Structure holds the coordinates of an image that was a match
 * with the mask image. */
typedef struct Match {
	int xStart;
	int xEnd;
	int yStart;
	int yEnd;
	double correlation;
} Match;

/* Reads the name of the file dynamically. */
char* readFileName() {
	char *array = calloc(2, sizeof(char));
	int len = 0;
	int maxSize = 2;

	char c = getchar();
	while((c = getchar()) != '\n') {
		if(len + 1 == maxSize) {
			maxSize *= 2;
			array = realloc(array, maxSize * sizeof(char));
		}
		array[len] = c;
		len++;
	}

	return array;
}

/* Compares 2 arrays before correlation. If they have different sizes,
 * they will be scaled up/down until they have the same length.
 * When the 2 arrays have the same length, they can be correlated. */
double correlateArrays(Array anglesF1T1, Array anglesF2T2) {
	double corr = -1.0; 

	Array copy1 = copyArray(anglesF1T1);
	Array copy2 = copyArray(anglesF2T2);

	/* Ensure arrays have the same length. */
	if(copy1.length > copy2.length && copy2.length != 0) interpolate(&copy1, &copy2);
	else if(copy1.length < copy2.length && copy1.length != 0) interpolate(&copy2, &copy1);

	/* Correlation with Pearson Correlator. */
	if(copy1.length == copy2.length) {
		// fprintf(stdout, "Arrays have the same length = %d\n", copy1.length);
		// fprintf(stdout, "First:\n"); printArray(copy1);
		// fprintf(stdout, "Second:\n"); printArray(copy2);

		corr = correlation(copy1, copy2);
	} else fprintf(stderr, "The 2 arrays do not have the same length!\n\n");

	freeArray(copy1);
	freeArray(copy2);

	return corr;
}

int getCurrentRow(PGMImage image, int threshold, int prevRow) {
	int currentRow = prevRow - 1;
	int count;
	int countThreshold = image.height / 5;

	while(currentRow+1 < image.height) {
		count = 0;
		for(int j=0; j < image.width; j++) {
			if(image.data[currentRow+1][j] < threshold) count++;
		}
		if(count < countThreshold) currentRow++;
		else break;
	}
	return currentRow;
}

int getNextRow(PGMImage image, int threshold, int currentRow) {
	int nextRow = currentRow;
	int minCount = image.width;
	int countThreshold = image.height / 5;
	int passedTheRow = 0;

	while(nextRow+1 < image.height) {
		int count = 0;
		for(int j=0; j < image.width; j++) {
			if(image.data[nextRow+1][j] < threshold) count++;
		}

		if(count > countThreshold && !passedTheRow) nextRow++;
		else if(count <= minCount) {
			minCount = count;
			nextRow++;
			passedTheRow = 1;
		} else break;
	}

	return nextRow;
}

int getCurrentCol(PGMImage image, int threshold, int currentRow, int nextRow, int prevCol) {
	int currentCol = prevCol - 1;
	int count;
	int countThreshold = (nextRow - currentRow) / 10;

	while(currentCol+1 < image.width) {
		count = 0;
		for(int i=currentRow; i < nextRow; i++) {
			if(image.data[i][currentCol+1] < threshold) count++;
		}

		if(count <= countThreshold) currentCol++;
	 	else break;
	}

	return currentCol;
}

int getNextCol(PGMImage image, int threshold, int currentRow, int nextRow, int currentCol) {
	int nextCol = currentCol;
	int minCount = image.height;
	int countThreshold = (nextRow - currentRow) / 10;
	int passedTheLetter = 0;

	while(nextCol+1 < image.width) {
		int count = 0;
		for(int i = currentRow; i < nextRow; i++) {
			if(image.data[i][nextCol+1] < threshold) count++;
		}

		if(count > countThreshold && !passedTheLetter) nextCol++;
		else if(count <= minCount) {
			minCount = count;
			nextCol++;
			passedTheLetter = 1;
		} else break;
	}

	return nextCol;
}

void thresholdOneImage() {
	Array angles;
	char* filename;
	int threshold;
	PGMImage image;

	/* Read name of the file. */
	fprintf(stdout, "Usage:\n");
	fprintf(stdout, "  File Name: ");
	filename = readFileName();

	/* Read threshold. */
	fprintf(stdout, "  Threshold: ");
	scanf("%d", &threshold);

	/* Read the image, convert it to a 1D array and print the array. */
	putchar('\n');
	image = readPGM(filename);
	angles = createContour(image, threshold);
	fprintf(stdout, "%s: \n", filename);
	printArray(angles);

	/* Free memory. */
	freeArray(angles);
	freePGM(image);
	free(filename);
}

void correlateTwoImages() {
	Array anglesF1T1, anglesF2T2;
	char *filename, *filename2;
	int threshold, threshold2;
	PGMImage image;
	double correlation;

	/* Read First Image. */
	fprintf(stdout, "Usage:\n");
	fprintf(stdout, "  File Name F1: ");
	filename = readFileName();

	fprintf(stdout, "  Threshold T1: ");
	scanf("%d", &threshold);

	/* Read Second Image. */
	fprintf(stdout, "  File Name F2: ");
	filename2 = readFileName();

	fprintf(stdout, "  Threshold T2: "); 
	scanf("%d", &threshold2);

	/* Create contour lines. */
	putchar('\n');
	image = readPGM(filename);
	anglesF1T1 = createContour(image, threshold);
	freePGM(image);

	image = readPGM(filename2);
	anglesF2T2 = createContour(image, threshold2);
	freePGM(image);

	/* Correlate. */
	correlation = correlateArrays(anglesF1T1, anglesF2T2);
	fprintf(stdout, "\nCorrelation: %lf\n\n", correlation);

	/* Free memory. */
	freeArray(anglesF1T1);
	freeArray(anglesF2T2);
	free(filename);
	free(filename2);
}

void searchForBestThreshold() {
	Array anglesF1T1, anglesF2T2;
	char *filename, *filename2;
	int threshold, bestThreshold;
	PGMImage image;
	double correlation, maxCorr;

	/* Read First Image. */
	fprintf(stdout, "Usage:\n");
	fprintf(stdout, "  File Name F1: ");
	filename = readFileName();

	fprintf(stdout, "  Threshold T1: ");
	scanf("%d", &threshold);

	/* Read Second Image - only the file name. */
	fprintf(stdout, "  File Name F2: ");
	filename2 = readFileName();

	/* Create contour line of the image. */
	putchar('\n');
	image = readPGM(filename);
	anglesF1T1 = createContour(image, threshold);
	freePGM(image);

	/* It will test all relevant thresholds, printing the best match. */
	maxCorr = 0.0;
	image = readPGM(filename2);
	for(int i=100; i < 170; i++) {
		anglesF2T2 = createContour(image, i);
		correlation = correlateArrays(anglesF1T1, anglesF2T2);
		if(correlation > maxCorr) {
			maxCorr = correlation;
			bestThreshold = i;
		}

		freeArray(anglesF2T2);
	}
	fprintf(stdout, "Best Correlation = %lf with threshold = %d\n\n", maxCorr, bestThreshold);

	/* Free memory. */
	freeArray(anglesF1T1);
	freePGM(image);
	free(filename);
	free(filename2);
}

void countLetterOccurences() {
	Array anglesF1T1, anglesF2T2;
	char *filename, *filename2;
	PGMImage image;
	int prevRow, prevCol, currentRow, currentCol, nextRow, nextCol;
	int threshold, threshold2, countLetters;
	double correlation, correlationThreshold;

	/* Init arrays of matches. */
	Match *matches = calloc(1, sizeof(Match));
	int countMatches = 0;
	int matchesMaxSize = 1;

	/* Read Image. */
	fprintf(stdout, "Usage:\n");
	fprintf(stdout, "  File Name: ");
	filename = readFileName();

	fprintf(stdout, "  Threshold: ");
	scanf("%d", &threshold);

	/* Read Page. */
	fprintf(stdout, "  Page Name: ");
	filename2 = readFileName();

	fprintf(stdout, "  Threshold: ");
	scanf("%d", &threshold2);

	/* Set the correlation level. */
	fprintf(stdout, "  Correlation: ");
	scanf("%lf", &correlationThreshold);

	/* Create contour line of the image. */
	putchar('\n');
	image = readPGM(filename);
	anglesF1T1 = createContour(image, threshold);
	freePGM(image);

	/* Split page in multiple components. */
	image = readPGM(filename2);
	countMatches = 0;
	countLetters = 0;
	prevRow=0;
	while(prevRow < image.height) {
		prevCol=0;

		/* Get next row. */
		currentRow = getCurrentRow(image, threshold2, prevRow);
		nextRow = getNextRow(image, threshold2, currentRow);

		while(prevCol < image.width) {
			/* Get next column. */
			currentCol = getCurrentCol(image, threshold2, currentRow, nextRow, prevCol);
			nextCol = getNextCol(image, threshold2, currentRow, nextRow, currentCol);

			printf("%d  %d  %d  %d\n", currentRow, nextRow, currentCol, nextCol);

			/* Extract letter from page. */
			PGMImage subImage = extractSubImage(image, currentRow, currentCol, nextRow, nextCol);
			PGMImage bordedSubImage = getBordedImage(subImage);

			/* Create contour of the extracted letter. */
			anglesF2T2 = createContour(bordedSubImage, threshold2);
			
			/* Correlate. */
			correlation = correlateArrays(anglesF1T1, anglesF2T2);
			fprintf(stdout, "Correlation = %lf\n\n", correlation);

			if(correlation >= correlationThreshold) {
				if(countMatches == matchesMaxSize) {
					matchesMaxSize *= 2;
					matches = realloc(matches, matchesMaxSize * sizeof(Match));
				}

				matches[countMatches].xStart = currentRow;
				matches[countMatches].xEnd = nextRow;
				matches[countMatches].yStart = currentCol;
				matches[countMatches].yEnd = nextCol;
				matches[countMatches].correlation = correlation;

				countMatches++;
			}

			prevCol = nextCol;
			countLetters++;

			/* Free temp memory. */
			freeArray(anglesF2T2);
			freePGM(subImage);
			freePGM(bordedSubImage);

			//break;

			if(currentCol == nextCol) break;
		}

		prevRow = nextRow;		

		if(currentRow == nextRow) break;
	}

	/* Printing results. */
	fprintf(stdout, "\n\n");
	fprintf(stdout, "Letters verified = %d\n", countLetters);
	fprintf(stdout, "Matches = %d\n", countMatches);
	for(int i=0; i<countMatches; i++) {
		fprintf(stdout, "  Match #%d: %d %d %d %d -> corr = %lf\n", i, matches[i].xStart, matches[i].xEnd, matches[i].yStart, matches[i].yEnd, matches[i].correlation);
	}

	/* Free memory. */
	freeArray(anglesF1T1);
	freePGM(image);
	free(filename);
	free(filename2);
	free(matches);
}

/* Main function. Executes the program according to the input. */
int main(int argc, char** argv) {
	fprintf(stdout, "Type in your choice:\n"
		   			"  1 - test threshold on image\n"
		   			"  2 - correlate 2 images\n"
		   			"  3 - search for best threshold value\n"
		   			"  4 - count occurences of a letter in a page\n\n"
		   			"  0 - EXIT\n\n"
		   			"Answer: ");

	int choice;
	scanf("%d", &choice);

	fprintf(stdout, "\n");

	switch(choice) {
		case 1:
			thresholdOneImage();
			break;

		case 2:
			correlateTwoImages();
			break;

		case 3:
			searchForBestThreshold();
			break;

		case 4:
			countLetterOccurences();
			break;

		default: break; /* Case 0 - Simply exit. */
	}

	return 0;
}
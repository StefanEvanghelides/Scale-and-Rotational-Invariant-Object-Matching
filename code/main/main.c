#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pgm/pgm.h"
#include "array/array.h"
#include "contour/contour.h"
#include "correlation/correlation.h"
#include "interpolation/interpolation.h"

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
		} else if(minCount < image.width / 8) break;
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
		} else if(minCount < image.width / 8) break;
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

void thresholdOneImage() {
	Array angles;
	char* filename;
	int threshold;
	PGMImage image;

	/* Read name of the file. */
	fprintf(stdout, "Usage:\n");
	fprintf(stdout, "  File Name: ");
	filename = calloc(100, sizeof(char)); 
	scanf("%s", filename);

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
	filename = calloc(100, sizeof(char)); 
	scanf("%s", filename);

	fprintf(stdout, "  Threshold T1: ");
	scanf("%d", &threshold);

	/* Read Second Image. */
	fprintf(stdout, "  File Name F2: ");
	filename2 = calloc(100, sizeof(char)); 
	scanf("%s", filename2);

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
	filename = calloc(100, sizeof(char)); 
	scanf("%s", filename);

	fprintf(stdout, "  Threshold T1: ");
	scanf("%d", &threshold);

	/* Read Second Image - only the file name. */
	fprintf(stdout, "  File Name F2: ");
	filename2 = calloc(100, sizeof(char)); 
	scanf("%s", filename2);


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
	int threshold, threshold2, count;
	double correlation;

	/* Read Image. */
	fprintf(stdout, "Usage:\n");
	fprintf(stdout, "  File Name: ");
	filename = calloc(100, sizeof(char)); 
	scanf("%s", filename);

	fprintf(stdout, "  Threshold: ");
	scanf("%d", &threshold);

	/* Read Page. */
	fprintf(stdout, "  Page Name: ");
	filename2 = calloc(100, sizeof(char)); 
	scanf("%s", filename2);

	fprintf(stdout, "  Threshold: ");
	scanf("%d", &threshold2);

	/* Set the correlation level. */
	fprintf(stdout, "  Correlation: ");
	scanf("%lf", &correlation);

	/* Create contour line of the image. */
	putchar('\n');
	image = readPGM(filename);
	anglesF1T1 = createContour(image, threshold);
	freePGM(image);

	/* Split page in multiple components. */
	image = readPGM(filename2);
	prevRow=0; prevCol=0;
	count = 0;
	while(prevRow < image.height) {
		/* Get next row. */
		currentRow = getCurrentRow(image, threshold2, prevRow);
		nextRow = getNextRow(image, threshold2, currentRow);

		printf("currentRow = %d, nextRow = %d\n", currentRow, nextRow);

		while(prevCol < image.width) {
			/* Get next column. */
			currentCol = getCurrentCol(image, threshold2, currentRow, nextRow, prevCol);
			nextCol = getNextCol(image, threshold2, currentRow, nextRow, currentCol);

			printf("currentCol = %d, nextCol = %d\n", currentCol, nextCol);


			/* Extract letter from page. */
			PGMImage subImage = extractSubImage(image, currentRow, currentCol, nextRow, nextCol);

			/* Create contour of the extracted letter. */
			anglesF2T2 = createContour(subImage, threshold2);
			/* Correlate. */
			
			correlation = correlateArrays(anglesF1T1, anglesF2T2);
			fprintf(stdout, "Correlation = %lf\n", correlation);

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
}

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
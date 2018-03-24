#include <stdio.h>
#include <stdlib.h>
#include "../../main/pgm/pgm.h"

void countBlackRows(PGMImage image, char *filename, int threshold) {
	FILE *file = fopen(filename, "wb");
	if(file == NULL) {
		fprintf(stderr, "Error: Unable to open file %s.\n\n", filename);
		exit(-1);
	}

	int countThreshold = image.height / 5;
	int countRows = -1;
	int countZeroNext = 1;
	for(int i=0; i<image.height; i++) {
		int count = 0;
		for(int j=0; j<image.width; j++) {
			if(image.data[i][j] < threshold) {
				count++;
				if(count >= countThreshold) countZeroNext = 1;
			}
		}
	
		fprintf(file, "Row #%d = %d\n", i, count);
		if(count < countThreshold && countZeroNext) {
			countRows++;
			countZeroNext = 0;
		}
	}

	fprintf(file, "Rows count = %d\n", countRows);

	fclose(file);
}

void countBlackCols(PGMImage image, char *filename, int threshold, int startRow, int endRow) {
	FILE *file = fopen(filename, "wb");
	if(file == NULL) {
		fprintf(stderr, "Error: Unable to open file %s.\n\n", filename);
		exit(-1);
	}

	int countThreshold = (endRow - startRow) / 10;
	int countLetters = -1;
	int countZeroNext = 1;
	for(int j=0; j<image.width; j++) {
		int count = 0;
		for(int i=startRow; i<endRow; i++) {
			if(image.data[i][j] < threshold) {
				count++;
				if(count >= countThreshold) countZeroNext = 1;
			}
		}
	
		fprintf(file, "Col #%d = %d\n", j, count);

		if(count < countThreshold && countZeroNext) {
			countLetters++;
			countZeroNext = 0;
		}
	}

	fprintf(file, "Letters count = %d\n", countLetters);	

	fclose(file);
}

int main(int argc, char ** argv) {
	char imageName[20], outputRows[20], outputCols[20];
	int threshold, startRow, endRow;

	printf("Image name: "); scanf("%s", imageName);
	printf("Threshold: "); scanf("%d", &threshold);
	printf("Output File Rows: "); scanf("%s", outputRows);
	printf("Output File Cols: "); scanf("%s", outputCols);
	printf("Starting Row: "); scanf("%d", &startRow);
	printf("Ending Row: "); scanf("%d", &endRow);

	PGMImage image = readPGM(imageName);

	countBlackRows(image, outputRows, threshold);
	countBlackCols(image, outputCols, threshold, startRow, endRow);

	return 0;
}
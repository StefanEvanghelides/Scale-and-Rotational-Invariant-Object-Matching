#include <stdio.h>
#include <stdlib.h>
#include "../../main/pgm/pgm.h"

void countBlackRows(PGMImage image, char *filename, int threshold) {
	FILE *file = fopen(filename, "wb");
	if(file == NULL) {
		fprintf(stderr, "Error: Unable to open file %s.\n\n", filename);
		exit(-1);
	}

	for(int i=0; i<image.height; i++) {
		int count = 0;
		for(int j=0; j<image.width; j++) {
			if(image.data[i][j] < threshold) count++;
		}
	
		fprintf(file, "Row #%d = %d\n", i, count);
	}

	fclose(file);
}

void countBlackCols(PGMImage image, char *filename, int threshold) {
	FILE *file = fopen(filename, "wb");
	if(file == NULL) {
		fprintf(stderr, "Error: Unable to open file %s.\n\n", filename);
		exit(-1);
	}

	for(int j=0; j<image.width; j++) {
		int count = 0;
		for(int i=0; i<image.height; i++) {
			if(image.data[i][j] < threshold) count++;
		}
	
		fprintf(file, "Col #%d = %d\n", j, count);
	}

	fclose(file);
}

int main(int argc, char ** argv) {
	char imageName[20], outputRows[20], outputCols[20];
	int threshold;

	printf("Image name: "); scanf("%s", imageName);
	printf("Threshold: "); scanf("%d", &threshold);
	printf("Output File Rows: "); scanf("%s", outputRows);
	printf("Output File Cols: "); scanf("%s", outputCols);

	PGMImage image = readPGM(imageName);

	countBlackRows(image, outputRows, threshold);
	countBlackCols(image, outputCols, threshold);

	return 0;
}
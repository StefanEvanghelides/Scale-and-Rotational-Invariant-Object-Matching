#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm_io.h"

/* Initialize the PGM function. */
PGMImage initializePGMImage(int width, int height, int maxVal) {
	PGMImage image;
	int row, col;
	
	image.width = width;
	image.height = height;
	image.maxVal = maxVal;
	
	/* Initialize 2D array for reading the data.*/
	image.data = malloc(height * sizeof(int*));
	image.data[0] = malloc(width * height * sizeof(int*));
	for(row = 1; row < height; row++) {
		image.data[row] = image.data[row-1] + image.width;
	}
	
	return image;
}

/* Read PGM file. */
PGMImage readPGM (char filename[]) {
	PGMImage image;
	FILE *file;
	int row,col,type;
	int width, height, maxVal;
	
	/* Open file. */
	file = fopen(filename, "rb");
	if(file == NULL) {
		fprintf(stderr, "Error: Unable to open file %s.\n\n", filename);
		exit(-1);
	}
	
	/* Get the type of file. */
	if(getc(file) != 'P' && getc(file) != '5') {
		fprintf(stderr, "Error: Not a valid P5 type PGM file.\n\n");
		exit(-1);
	}
	
	/* Skip comments and new line characters. */
	while(getc(file) != '\n');
    while (getc(file) == '#') {
      while (getc(file) != '\n');         
    }
	fseek(file, -1, SEEK_CUR);
	
	/* Read PGM image's width, height and maximum value */
	fprintf(stdout, "Reading image...\n");
	fscanf(file, "%d", &width);
	fscanf(file, "%d", &height);
	fscanf(file, "%d", &maxVal);
	fprintf(stdout, "Width=%d  Height=%d  MaxVal=%d\n", width, height, maxVal);
	
	/* Initialize the PGM image. */
	image = initializePGMImage(width, height, maxVal);
	
	/* Reading the data. */
	while(getc(file) != '\n'); 
	for(row = 0; row < height; row++) {
		for(col = 0; col < width; col++) {
			image.data[row][col] = getc(file);
		}
	}
	
	/* Close file. */
	fclose(file);
	fprintf(stdout, "Finished reading the file.\n\n");
	
	return image;
}


/* Write PGM file. 
   Prerequisite: *img variable must be initialized.*/
void writePGM(char filename[], PGMImage image) {
	FILE *file;
	int row, col;
	
	/* Open file. */
	file = fopen(filename, "wb");
	if(file == NULL) {
		fprintf(stderr, "Error: Unable to open file %s.\n\n", filename);
		exit(-1);
	}
	
	/* Writing image's details. */
	fprintf(stdout, "Writing the image in P5 format...\n");
	fprintf(file, "P5\n");
	fprintf(file, "# Creator: Stefan Evanghelides\n");
	fprintf(file, "%d %d\n", image.width, image.height);
	fprintf(file, "%d\n", image.maxVal);
	
	/* Writing image's data. */
	for(row = 0; row < image.height; row++) {
		for(col = 0; col < image.width; col++) {
			fprintf(file, "%c", (unsigned char) image.data[row][col]);
		}
	}
	
	/* Close file. */
	fclose(file);
	fprintf(stdout, "Finished writing the image.\n");
}
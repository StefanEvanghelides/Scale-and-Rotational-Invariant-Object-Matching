#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm.h"

/* Initialize the PGM function. */
PGMImage initializePGMImage(int type, int width, int height, int maxVal) {
	PGMImage image;
	int row;
	
	image.type = type;
	image.width = width;
	image.height = height;
	image.maxVal = maxVal;
	
	/* Initialize 2D array for reading the data.*/
	image.data = calloc(height, sizeof(int*));
	image.data[0] = calloc(width * height, sizeof(int*));
	for(row = 1; row < height; row++) {
		image.data[row] = image.data[row-1] + image.width;
	}
	
	return image;
}

/* Read PGM file. */
PGMImage readPGM (char filename[]) {
	PGMImage image;
	FILE *file;
	int row, col, next_value;
	int width, height, maxVal;
	char type[2]; /* For Types P2 and P5. */

	/* Open file. */
	file = fopen(filename, "rb");
	if(file == NULL) {
		fprintf(stderr, "Error: Unable to open file %s.\n\n", filename);
		exit(-1);
	}
	
	/* Get the type of file. */
	type[0] = getc(file);
	type[1] = getc(file);
	if(!(type[0] == 'P' && (type[1] == '2' || type[1] == '5'))) {
		fprintf(stderr, "Error: Not a valid P2 or P5 type PGM file.\n\n"); 
		exit(-1);
	}
	
	/* Skip comments and new line characters. */
	while(getc(file) != '\n');
    while (getc(file) == '#') {
      while (getc(file) != '\n');         
    }
	fseek(file, -1, SEEK_CUR);
	
	/* Read PGM image's width, height and maximum value */
	fscanf(file, "%d", &width);
	fscanf(file, "%d", &height);
	fscanf(file, "%d", &maxVal);
	
	/* Initialize the PGM image. */
	image = initializePGMImage(atoi(type+1), width, height, maxVal);
	
	/* Reading the data. */
	while(getc(file) != '\n');
	if(image.type == 5) {
		for(row = 0; row < height; row++) {
			for(col = 0; col < width; col++) {
				image.data[row][col] = getc(file);
			}
		}
	} else { /* So type is P2 */
		for(row = 0; row < height; row++) {
			for(col = 0; col < width; col++) {
				fscanf(file, "%d", &next_value);
				image.data[row][col] = next_value;
			}
		}
	}
	
	/* Close file. */
	fclose(file);
	
	return image;
}


/* Write PGM file. 
   Prerequisite: *img variable must be initialized.*/
void writePGM(PGMImage image, char filename[]) {
	FILE *file;
	int row, col;
	
	/* Open file. */
	file = fopen(filename, "wb");
	if(file == NULL) {
		fprintf(stderr, "Error: Unable to open file %s.\n\n", filename);
		exit(-1);
	}
	
	/* Writing image's details. */
	fprintf(file, "P%d\n", image.type);
	fprintf(file, "# Creator: Stefan Evanghelides\n");
	fprintf(file, "%d %d\n", image.width, image.height);
	fprintf(file, "%d\n", image.maxVal);
	
	/* Writing image's data. */
	if(image.type == 5) {
		for(row = 0; row < image.height; row++) {
			for(col = 0; col < image.width; col++) {
				fprintf(file, "%c", (unsigned char) image.data[row][col]);
			}
		}
	} else { /* So type is P2. */
		for(row = 0; row < image.height; row++) {
			for(col = 0; col < image.width; col++) {
				fprintf(file, "%d\n", image.data[row][col]);
			}
		}
	}
	
	/* Close file. */
	fclose(file);
}

/* Extracts subimage from a larger image.
 * PRE: nextRow and colRow should not be out of bounds. */
PGMImage extractSubImage(PGMImage image, int currentRow, int currentCol, int nextRow, int nextCol) {
	int type = image.type;
	int width = nextCol - currentCol;
	int height = nextRow - currentRow;
	int maxVal = image.maxVal;

	PGMImage subImage = initializePGMImage(type, width, height, maxVal);

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			subImage.data[i][j] = image.data[i + currentRow][j + currentCol];
		}
	}

	return subImage;
}

PGMImage getBordedImage(PGMImage image) {
	int type = image.type;
	int height = image.height + 2;
	int width = image.width + 2;
	int maxVal = image.maxVal;

	PGMImage newImage = initializePGMImage(type, width, height, maxVal);

	/* Set up the border. */
	for(int j=0; j<width; j++) { // Top and Bottom border
		newImage.data[0][j] = 200;
		newImage.data[height-1][j] = 200;
	}

	for(int i=0; i<height; i++) { // Left and Right
		newImage.data[i][0] = 200;
		newImage.data[i][width-1] = 200;
	}

	/* Copy the rest of the content. */
	for(int i=0; i<image.height; i++) {
		for(int j=0; j<image.width; j++) {
			newImage.data[i+1][j+1] = image.data[i][j];
		}
	}

	return newImage;
}

void drawBox(PGMImage *image, int rowStart, int rowEnd, int colStart, int colEnd) {
	for(int i = rowStart; i < rowEnd; i++) {  // Left and Right
		image->data[i][colStart] = 255;
		image->data[i][colEnd] = 255;
	}

	for(int j = colStart; j < colEnd; j++) { // Top and Bottom border
		image->data[rowStart][j] = 255;
		image->data[rowEnd][j] = 255;
	}
}


/* Prints Image on the standard output. */
void printPGM(PGMImage image) {
	int row, col, nextValue;
	
	fprintf(stdout, "Printing Image...\n");
	fprintf(stdout, "Type=P%d, ", image.type);
	fprintf(stdout, "Width=%d, ", image.width);
	fprintf(stdout, "Height=%d, ", image.height);
	fprintf(stdout, "MxVal=%d\n", image.maxVal);
	fprintf(stdout, "Data:\n");

	for(row = 0; row < image.height; row++) {
		for(col = 0; col < image.width; col++) {
			nextValue = image.data[row][col];
			if(nextValue < 100) fprintf(stdout, " ");
			if(nextValue < 10)  fprintf(stdout, " ");
			fprintf(stdout, "%d ", nextValue);
		}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
}

/* Free the PGM image from memory. */
void freePGM(PGMImage image) {
	free(image.data[0]);
	free(image.data);
}

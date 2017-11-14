#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm_io.h"

/* Read PGM file. 
   This function consideres the *img variable uninitialized.*/
void readPGM (char filename[], PGMImage *img) {
	FILE *file;
	unsigned char ch;
	int row,col,type;
	
	/* Open file. */
	file = fopen(filename, "rb");
	if(file == NULL) {
		fprintf(stderr, "Error: Unable to open file %s.\n\n", filename);
		exit(-1);
	}
	
	/* Get the type of file. */
	ch = getc(file);
	if(ch != 'P') {
		fprintf(stderr, "Error: Not a valid PGM file.\n\n");
		exit(-1);
	}
	ch = getc(file); // here is an integer represented as char 48 == '0'
	type = ch - 48;
	if(type != 5 && type != 2) {
		fprintf(stderr, "Error: Only P5 and P2 PGM files supported.\n\n");
		exit(-1);
	}
	
	/* Skip comments and new line characters. */
	while(ch = getc(file) != '\n');
    while (ch = getc(file) == '#') {
      while (ch = getc(file) != '\n');         
    }
	fseek(file, -1, SEEK_CUR); /* Windows solution. Will test on Unix as well.*/
	
	/* Read PGM image's width, height and maximum value */
	fprintf(stdout, "Reading image...\n");
	fscanf(file, "%d", &(img->width));
	fscanf(file, "%d", &(img->height));
	fscanf(file, "%d", &(img->maxVal));
	fprintf(stdout, "Width=%d  Height=%d  MaxVal=%d\n", img->width, img->height, img->maxVal);
	
	/* Initialize 2D array for reading the data.*/
	img->data = malloc(img->height * sizeof(int*));
	img->data[0] = malloc(img->width * img->height * sizeof(int*));
	for(row = 1; row < img->height; row++) {
		img->data[row] = img->data[row-1] + img->width;
	}
	
	/* Reading the data. */
	while(getc(file) != '\n'); 
	for(row = 0; row < img->height; row++) {
		for(col = 0; col < img->width; col++) {
			ch = getc(file);
			img->data[row][col] = ch;
		}
	}
	
	/* Close file. */
	fclose(file);
	fprintf(stdout, "Finished reading the file.\n\n");
}


/* Write PGM file. 
   Prerequisite: *img variable must be initialized.*/
void writePGM(char filename[], PGMImage *img) {
	FILE *file;
	int row, col;
	unsigned char ch;
	
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
	fprintf(file, "%d %d\n", img->width, img->height);
	fprintf(file, "%d\n", img->maxVal);
	
	/* Writing image's data. */
	for(row = 0; row < img->height; row++) {
		for(col = 0; col < img->width; col++) {
			ch = (unsigned char) img->data[row][col];
			fprintf(file, "%c", ch);
		}
	}
	
	/* Close file. */
	fclose(file);
	fprintf(stdout, "Finished writing the image.\n");
}
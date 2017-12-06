#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../pgm/pgm.h"

char *getFilename(char* initialFilename, int newType) {
	int length = strlen(initialFilename);
	char* filename = malloc((length+5)*sizeof(char));
	
	/* Copies the current name. */
	int i;
	for(i=0; i<length-4; i++) {
		filename[i] = initialFilename[i];
	}

	/* Add extra info regarding rotation. */
	filename[length - 4] = '_';
	filename[length - 3] = 'R';
	filename[length - 2] = '9';
	filename[length - 1] = '0';
	
	/* Finish by adding the extension. */
	for(i=length-4; i<length; i++) {
		filename[i+4] = initialFilename[i];
	}
	filename[length+4] = '\0';

	return filename;
}

/* Rotate the current image by 90 degres. */
PGMImage rotate(PGMImage image) {
	PGMImage rotated = initializePGMImage(image.type, image.height, image.width, image.maxVal);

	for(int row = 0; row < rotated.height; row++) {
		for(int col = 0; col < rotated.width; col++) {
			rotated.data[row][col] = image.data[col][image.width - row - 1];
		}
	}

	return rotated;
}

int main(int argc, char** argv) {
	if(argc < 2) {
		fprintf(stderr, "ERROR: Missing the name of the file!\nUsage: ./ROTATE <file_name>\n\n");
		exit(-1);
	}
	/* Read the PGM Image. */
	PGMImage image = readPGM(argv[1]);

	/* Rotate the image. */
	PGMImage rotated = rotate(image);

	/* Print rotated. */
	printImage(rotated);

	/* Create the name of the file. */
	char *filename = getFilename(argv[1], image.type);

	/* Save the current image. */
	writePGM(rotated, filename);

	/* Free memory. */
	freePGM(image);
	freePGM(rotated);
}
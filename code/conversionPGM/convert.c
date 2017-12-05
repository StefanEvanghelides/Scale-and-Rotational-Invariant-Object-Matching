#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../pgm/pgm.h"

char *getFilename(char* initialFilename, int newType) {
	int length = strlen(initialFilename);
	char* filename = malloc((length+4)*sizeof(char));
	
	/* Copies the current name. */
	int i;
	for(i=0; i<length-4; i++) {
		filename[i] = initialFilename[i];
	}

	/* Add extra info regarding the new type. */
	filename[length - 4] = '_';
	filename[length - 3] = 'P';
	filename[length - 2] = newType + '0';
	
	/* Finish by adding the extension. */
	for(i=length-4; i<length; i++) {
		filename[i+3] = initialFilename[i];
	}
	filename[length+3] = '\0';

	return filename;
}

int main (int argc, char ** argv) {
	PGMImage image;
	char *filename;

	/* Check arguments. */
	if(argc < 2) {
		fprintf(stderr, "File name missing! Use the following syntax:\n\
			    ./CONVERT <filename>\n");
		exit(-1);
	}
	
	/* Read the image. */
	image = readPGM(argv[1]);

	/* Change type: If type was 2, it will be 5; If type was 5, it will be 2. */
	image.type = (image.type + 3) % 6;

	/* Create the name of the file. */
	filename = getFilename(argv[1], image.type);

	/* Save the current image. */
	writePGM(image, filename);

	/* Free memory. */
	free(filename);

	return 0;
}
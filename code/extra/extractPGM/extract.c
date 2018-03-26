#include <stdio.h>
#include <stdlib.h>
#include "../../main/pgm/pgm.h"


/* Extracts subimage from a larger image.
 * PRE: nextRow and colRow should not be out of bounds. */
PGMImage extractSub(PGMImage image, int currentRow, int currentCol, int nextRow, int nextCol) {
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

int main(int argc, char ** argv) {
	char imageName[20];
	int currentRow, currentCol, nextRow, nextCol;

	printf("Image name: "); scanf("%s", imageName);
	printf("Current Row: "); scanf("%d", &currentRow);
	printf("Next Row: "); scanf("%d", &nextRow);
	printf("Current Col: "); scanf("%d", &currentCol);
	printf("Next Col: "); scanf("%d", &nextCol);

	PGMImage image = readPGM(imageName);
	PGMImage subImage = extractSub(image, currentRow, currentCol, nextRow, nextCol);
	writePGM(subImage, "output.pgm");

	return 0;
}
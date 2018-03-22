#ifndef PGM_H
#define PGM_H

/* Type representing the PGM image. */
typedef struct PGMImage {
	int type;
	int width;
	int height;
	int maxVal;
	int** data;
} PGMImage;
  
/* Prototypes. */
PGMImage initializePGMImage(int type, int width, int height, int maxVal);
PGMImage readPGM (char filename[]);
void writePGM(PGMImage image, char filename[]);
PGMImage extractSubImage(PGMImage image, int currentrow, int currentCol, int nextRow, int nextCol);
void printImage(PGMImage image);
void freePGM(PGMImage image);

#endif
 
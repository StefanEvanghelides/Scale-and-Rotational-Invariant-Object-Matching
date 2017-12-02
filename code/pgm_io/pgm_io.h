#ifndef PGM_IO_H
#define PGM_IO_H

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

#endif
 
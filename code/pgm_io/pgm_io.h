#ifndef PGM_IO_H
#define PGM_IO_H

/* Type representing the PGM image. */
typedef struct PGMImage {
  int width;
  int height;
  int maxVal;
  int** data;
} PGMImage;
  
/* Prototypes. */
void readPGM (char filename[], PGMImage *img);
void writePGM(char filename[], PGMImage *img);


#endif
 
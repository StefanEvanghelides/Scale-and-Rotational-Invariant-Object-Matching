#ifndef PGM_IO_H
#define PGM_IO_H

typedef struct PGMImage {
  int maxVal;
  int width;
  int height;
  char** data;
} PGMImage;

  
/***prototypes**********************************************************/
/***********************************************************************/
  
void readPGM (char filename[], PGMImage *img);
void writePGM(char filename[], PGMImage *img);


#endif
 
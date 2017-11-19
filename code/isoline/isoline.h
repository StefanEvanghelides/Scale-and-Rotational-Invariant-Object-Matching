#ifndef ISOLINE_H
#define ISOLINE_H

/* Prototypes. */
PGMImage applyThreshold(PGMImage image, int threshold);
int getContourCase(int topLeft, int topRight, int bottomLeft, int bottomRight);
PGMImage createContour(PGMImage image);

#endif
 
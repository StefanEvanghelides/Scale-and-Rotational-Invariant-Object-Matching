#ifndef ISOLINE_H
#define ISOLINE_H

/* Prototypes. */
int getAngle(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold);
bool isEdge(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold);
int *createContour(PGMImage image, int threshold);

#endif
 
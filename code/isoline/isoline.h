#ifndef ISOLINE_H
#define ISOLINE_H

/* Direction macros. */
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

/* Prototypes. */
int getAngle(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold);
bool isEdge(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold);
Array createContour(PGMImage image, int threshold);

#endif
 
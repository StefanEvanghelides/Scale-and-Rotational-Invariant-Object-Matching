#ifndef ISOLINE_H
#define ISOLINE_H

/* Direction macros. */
#define UP 1
#define RIGHT 2
#define DOWN -1
#define LEFT -2

/* Prototypes. */
double getAngle(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold, int *firstPoint);
bool isEdge(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold);
bool reachedStartingPoint(int row, int col, int startX, int startY);
bool isInBounds(int row, int col, int height, int width);
Array createContour(PGMImage image, int threshold);

#endif
 
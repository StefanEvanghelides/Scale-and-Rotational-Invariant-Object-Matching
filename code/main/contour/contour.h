#ifndef CONTOUR_H
#define CONTOUR_H

/* Direction macros. */
#define NONE 0
#define UP 1
#define RIGHT 2
#define DOWN -1
#define LEFT -2

/* Pi */
#define pi 3.14159265358979323846

/* Only createContour function must be externalized. */
Array createContour(PGMImage image, int threshold);

#endif
 
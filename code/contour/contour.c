#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../pgm/pgm.h"
#include "../array/array.h"
#include "contour.h"

#define PI 3.14159265358979323846

/* Prints the direction of the point as a string */
void printPointDirection(int point) {
	switch(point) {
		case UP:
			fprintf(stdout, "UP");
			break;
		case RIGHT:
			fprintf(stdout, "RIGHT");
			break;
		case DOWN:
			fprintf(stdout, "DOWN");
			break;
		case LEFT:
			fprintf(stdout, "LEFT");
			break;
	}
}

/* Returns the position of the first point. The expected values are: UP, LEFT or DOWN. */
int getFirstPoint(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold) {
	int firstPoint;

	if(bottomRight < threshold) {
		firstPoint = DOWN;
		if (bottomLeft < threshold) firstPoint = LEFT;
		else if (topRight < threshold) firstPoint = UP;
	} else if (topRight < threshold) firstPoint = UP;

	return firstPoint;
}

/* Returns the second point, based on the first point and the 4 corners of the square. */
int getSecondPoint(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold, int firstPoint) {
	int secondPoint;

	switch(firstPoint) {
		case UP:
			if(topLeft < threshold) {
				if (bottomLeft < threshold && bottomRight < threshold) secondPoint = RIGHT;
				else if (bottomLeft < threshold) secondPoint = DOWN;
				else if (bottomRight < threshold) {
					secondPoint = LEFT;
					fprintf(stdout, "WARNING: Saddle point not properly dealt with! Currently it's set to ");
					printPointDirection(secondPoint);
					fprintf(stdout, ".\n");
				} else secondPoint = LEFT;
			} else if(topRight < threshold) {
				if (bottomLeft < threshold && bottomRight < threshold) secondPoint = LEFT;
				else if (bottomRight < threshold) secondPoint = DOWN;
				else if (bottomLeft < threshold) {
					secondPoint = RIGHT;
					fprintf(stdout, "WARNING: Saddle point not properly dealt with! Currently it's set to ");
					printPointDirection(secondPoint);
					fprintf(stdout, ".\n");
				} else secondPoint = RIGHT;
			}
			break;
		
		case RIGHT:
			if(topRight < threshold) {
				if (topLeft < threshold && bottomLeft < threshold) secondPoint = DOWN;
				else if (topLeft < threshold) secondPoint = LEFT;
				else if (bottomLeft < threshold) {
					secondPoint = UP;
					fprintf(stdout, "WARNING: Saddle point not properly dealt with! Currently it's set to ");
					printPointDirection(secondPoint);
					fprintf(stdout, ".\n");
				} else secondPoint = UP;
			} else if(bottomRight < threshold) {
				if (topLeft < threshold && bottomLeft < threshold) secondPoint = UP;
				else if (bottomLeft < threshold) secondPoint = LEFT;
				else if (topLeft < threshold) {
					secondPoint = DOWN;
					fprintf(stdout, "WARNING: Saddle point not properly dealt with! Currently it's set to ");
					printPointDirection(secondPoint);
					fprintf(stdout, ".\n");
				} else secondPoint = DOWN;
			}
			break;
		
		case DOWN:
			if(bottomLeft < threshold) {
				if (topLeft < threshold && topRight < threshold) secondPoint = RIGHT;
				else if (topLeft < threshold) secondPoint = UP;
				else if (topRight < threshold) {
					secondPoint = LEFT;
					fprintf(stdout, "WARNING: Saddle point not properly dealt with! Currently it's set to ");
					printPointDirection(secondPoint);
					fprintf(stdout, ".\n");
				} else secondPoint = LEFT;
			} else if(bottomRight < threshold) {
				if (topLeft < threshold && topRight < threshold) secondPoint = LEFT;
				else if (topRight < threshold) secondPoint = UP;
				else if (topLeft < threshold) {
					secondPoint = RIGHT;
					fprintf(stdout, "WARNING: Saddle point not properly dealt with! Currently it's set to ");
					printPointDirection(secondPoint);
					fprintf(stdout, ".\n");
				} else secondPoint = RIGHT;
			}
			break;
		
		case LEFT:
			if(topLeft < threshold) {
				if (topRight < threshold && bottomRight < threshold) secondPoint = DOWN;
				else if (topRight < threshold) secondPoint = RIGHT;
				else if (bottomRight < threshold) {
					secondPoint = UP;
					fprintf(stdout, "WARNING: Saddle point not properly dealt with! Currently it's set to ");
					printPointDirection(secondPoint);
					fprintf(stdout, ".\n");
				} else secondPoint = UP;
			} else if(bottomLeft < threshold) {
				if (topRight < threshold && bottomRight < threshold) secondPoint = UP;
				else if (bottomRight < threshold) secondPoint = RIGHT;
				else if (topRight < threshold) {
					secondPoint = DOWN;
					fprintf(stdout, "WARNING: Saddle point not properly dealt with! Currently it's set to ");
					printPointDirection(secondPoint);
					fprintf(stdout, ".\n");
				} else secondPoint = DOWN;
			}
			break;
	}

	return secondPoint;
}

/* Returns the coordinates for finding the next square. */
void findNextSquare(int direction, int *row, int *col) {
	switch(direction) {
		case UP:
			(*row)--;
			break;
		case RIGHT:
			(*col)++;
			break;
		case DOWN:
			(*row)++;
			break;
		case LEFT:
			(*col)--;
			break;
	}
}

/* Checks whether the 4 pixels form an edge. */
bool isEdge(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold) {
	return ((topLeft<threshold || topRight<threshold || bottomLeft<threshold || bottomRight<threshold)
		&& !(topLeft>threshold && topRight>threshold && bottomLeft>threshold && bottomRight>threshold)
		&& !(topLeft<threshold && topRight<threshold && bottomLeft<threshold && bottomRight<threshold));
}

/* Checks if the algorithm reached the starting point. */
bool reachedStartingPoint(int row, int col, int startX, int startY) {
	return (startX != -1 && startY != -1 && row == startX && col == startY);
}

/* Checks whether the values of row and col are within the limits. */
bool isInBounds(int row, int col, int height, int width) {
	return (row>=0 && row<height-1 && col>=0 && col<width-1);
}

/* Finds the location of the starting square, holding the coordinates of the top left corner. */
void findStartingSquareLocation(PGMImage image, int threshold, int *startX, int *startY) {
	int row, col;
	int topLeft, topRight, bottomLeft, bottomRight;
	bool stop;

	row = 0; col = 0; stop = false;
	while(row < image.height - 1) {
		while(col < image.width - 1) {
			topLeft     = image.data[row][col];         
			topRight    = image.data[row][col+1];      
			bottomLeft  = image.data[row+1][col];    
			bottomRight = image.data[row+1][col+1];

			if(isEdge(topLeft, topRight, bottomLeft, bottomRight, threshold)) {
				*startX = row;
				*startY = col;
				stop = true;
				break;
			}
			col++;
		}

		if(stop) break;

		col = 0;
		row++;
	}
}

double getAngle(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold, int firstPoint, int secondPoint) {
	double angle = 0.0;
	double normFirst, normSecond, delta;

	switch(firstPoint) {
		case UP:
			normFirst = fabs(topRight - threshold) / fabs(topRight - topLeft);
			if (secondPoint == DOWN) {
				normSecond = fabs(bottomRight - threshold) / fabs(bottomRight - bottomLeft);
				delta = normFirst - normSecond;
				angle = atan2(-1, delta);
			} else if (secondPoint == LEFT) {
				normFirst = 1 - normFirst;
				normSecond = fabs(topLeft - threshold) / fabs(bottomLeft - topLeft);
				angle = atan2(-normSecond, -normFirst);
			} else {
				normSecond = fabs(topRight - threshold) / fabs(topRight - bottomRight);
				angle = atan2(-normSecond, normFirst);
			}
			break;

		case DOWN:
			normFirst = fabs(bottomRight - threshold) / fabs(bottomRight - bottomLeft);
			if (secondPoint == UP) {
				normSecond = fabs(topRight - threshold) / fabs(topRight - topLeft);
				delta = normFirst - normSecond;
				angle = atan2(1, delta);
			} else if (secondPoint == LEFT) {
				normFirst = 1 - normFirst;
				normSecond = fabs(bottomLeft - threshold) / fabs(bottomLeft - topLeft);
				angle = atan2(normSecond, -normFirst);
			} else {
				normSecond = fabs(bottomRight - threshold) / fabs(topRight - bottomRight);
				angle = atan2(normSecond, normFirst);
			}
			break;

		case LEFT:
			normFirst = fabs(topLeft - threshold) / fabs(bottomLeft - topLeft);
			if (secondPoint == RIGHT) {
				normSecond = fabs(topRight - threshold) / fabs(topRight - bottomRight);
				delta = normFirst - normSecond;
				angle = atan2(delta, 1);
			} else if (secondPoint == UP) {
				normSecond = fabs(topLeft - threshold) / fabs(topLeft - topRight);
				angle = atan2(normFirst, normSecond);
			} else {
				normFirst = 1 - normFirst;
				normSecond = fabs(bottomLeft - threshold) / fabs(bottomLeft - bottomRight);
				angle = atan2(-normFirst, normSecond);
			}
			break;

		case RIGHT:
			normFirst = fabs(topRight - threshold) / fabs(bottomRight - topRight);
			if (secondPoint == LEFT) {
				normSecond = fabs(topLeft - threshold) / fabs(topLeft - bottomLeft);
				delta = normFirst - normSecond;
				angle = atan2(delta, -1);
			} else if (secondPoint == UP) {
				normSecond = fabs(topRight - threshold) / fabs(topLeft - topRight);
				angle = atan2(normFirst, -normSecond);
			} else {
				normFirst = 1 - normFirst;
				normSecond = fabs(bottomRight - threshold) / fabs(bottomLeft - bottomRight);
				angle = atan2(-normFirst, -normSecond);
			}
			break;

	}

	return angle;
}


/* This function will create the contour of an object, which will be represented as an 1D array of angles.
   These angles are based on the relative position of the threshold, applied to a group of 2X2 pixels.
   (see Marching Squares algorithm)  */
Array createContour(PGMImage image, int threshold) {
	int row, col, count;
	int topLeft, topRight, bottomLeft, bottomRight;
	int startX, startY, firstPoint, secondPoint;
	double currentAngle;
	Array angles;

	/* Initializes the array with the size 64. */
	initArray(&angles); 
	
	/* Find starting square. */
	findStartingSquareLocation(image, threshold, &startX, &startY);

	/* No edge detected. Stop the program. */
	if(!isInBounds(startX, startY, image.height, image.width)) { 
		fprintf(stderr, "No edge detected. Cannot compute contour!\n\n");
		return angles;
	}

	/* Walk along the contour, storing the angles of each edge. */
	row = startX; col = startY; count = 1; firstPoint = NONE; secondPoint = NONE;
	do {
		topLeft     = image.data[row][col];         
		topRight    = image.data[row][col+1];      
		bottomLeft  = image.data[row+1][col];    
		bottomRight = image.data[row+1][col+1]; 

		/* Compute the first point (if it was not already computed).
		   Otherwise, the first point will become the flipped version of the (previous) second point.*/
		if(firstPoint == NONE) {
			firstPoint = getFirstPoint(topLeft, topRight, bottomLeft, bottomRight, threshold);
		} else firstPoint = secondPoint * (-1); 

		secondPoint = getSecondPoint(topLeft, topRight, bottomLeft, bottomRight, threshold, firstPoint);

		currentAngle = getAngle(topLeft, topRight, bottomLeft, bottomRight, threshold, firstPoint, secondPoint);

		addElement(&angles, currentAngle);

		findNextSquare(secondPoint, &row, &col);

	} while(!reachedStartingPoint(row, col, startX, startY) && isInBounds(row, col, image.height, image.width)
		&& (++count) < image.height * image. width);

	fprintf(stdout, "Length = %d\n", count);

	/* Having the array of angles, compute the array of delta's. */
	int idx = angles.length-1;
	while (idx > 1) {
		angles.data[idx] -= angles.data[idx-1];
		idx--;
	}

	/* Smooth the Angles, normalize in the (-pi, pi) range. */
	for(int idx = 0; idx < angles.length; idx++) {
		if(angles.data[idx] > PI) angles.data[idx] -= 2 * PI;
		if(angles.data[idx] < -PI) angles.data[idx] += 2 * PI;
	}

	return angles;
}
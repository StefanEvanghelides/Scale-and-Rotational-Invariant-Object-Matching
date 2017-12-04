#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../pgm/pgm.h"
#include "array.h"
#include "contour.h"

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

/* Returns the angle of the current square and sets the direction for the new square. */
double getAngle(int topLeft, int topRight, int bottomLeft, int bottomRight, int threshold, int *firstPoint) {
	int secondPoint; /* Should take values: UP, Right, DOWN, LEFT. */
	double angle = 0.0;

	*firstPoint *= -1;

	if(*firstPoint == NONE) {
		*firstPoint = getFirstPoint(topLeft, topRight, bottomLeft, bottomRight, threshold);
		fprintf(stdout, "Starting FirstPoint = %d\n\n", *firstPoint);
	}

	secondPoint = getSecondPoint(topLeft, topRight, bottomLeft, bottomRight, threshold, *firstPoint);


	printPointDirection(*firstPoint);
	fprintf(stdout, " -> ");
	printPointDirection(secondPoint);
	fprintf(stdout, "\n");

	*firstPoint = secondPoint;

	return angle;
}

/* Returns the coordinates for finding the next square. */
void getNextSquare(int direction, int *row, int *col) {
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
	return (row>=0 && row<height && col>=0 && col<width);
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

/* This function will create the contour of an object,
   which will be represented as an 1D array of angles.
   These angles are based on the relative position of the threshold,
   applied to a group of 2X2 pixels.
   (see Marching Squares algorithm)  */
Array createContour(PGMImage image, int threshold) {
	int row, col, count;
	int topLeft, topRight, bottomLeft, bottomRight;
	int startX, startY, firstPoint;
	double currentAngle;
	Array angles;

	/* Initializes the array with the size 64. */
	initArray(&angles, 64); 
	
	/* Find starting square. */
	findStartingSquareLocation(image, threshold, &startX, &startY);

	/* No edge detected. Stop the program. */
	if(!isInBounds(startX, startY, image.height, image.width)) { 
		fprintf(stderr, "No edge detected. Cannot compute contour!\n\n");
		exit(-1);
	}

	/* Walk along the contour, storing the angles of each edge. */
	row = startX; col = startY; count = 0; firstPoint = NONE;
	do {
		if(count > image.width * image.height) { /* No cycle. just break without error. */
			fprintf(stdout, "WARNING: No cycle detected!\n\n");
			break;
		}

		topLeft     = image.data[row][col];         
		topRight    = image.data[row][col+1];      
		bottomLeft  = image.data[row+1][col];    
		bottomRight = image.data[row+1][col+1]; 

		currentAngle = getAngle(topLeft, topRight, bottomLeft, bottomRight, threshold, &firstPoint);

		fprintf(stdout, "Edge on the Square with the Top Left position (%d,%d); Angle=%lf\n\n", row, col, currentAngle);
		
		addElement(&angles, currentAngle);
		getNextSquare(firstPoint, &row, &col);
		count++;
	} while(!reachedStartingPoint(row, col, startX, startY) && isInBounds(row, col, image.height, image.width));

	fprintf(stdout, "Count = %d\n\n", count);

	return angles;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm_io/pgm_io.h"
#include "isoline/isoline.h"

int main(int argc, char** argv) {
	
	PGMImage image = readPGM("mask.pgm");
	PGMImage contour = createContour(image);
	writePGM(contour, "mask_contour.pgm");
	
	return 0;
}
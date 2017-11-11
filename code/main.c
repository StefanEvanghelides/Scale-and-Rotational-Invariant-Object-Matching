#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm_io/pgm_io.h"


int main(int argc, char** argv) {
	
	PGMImage image;
	
	readPGM("mask", &image);
	writePGM("mask_saved", &image);
	
	return 0;
}
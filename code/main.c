#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm_io/pgm_io.h"


int main(int argc, char** argv) {
	PGMImage image;
	
	readPGM("mask.pgm", &image);
	writePGM("mask_saved.pgm", &image);
	
	return 0;
}
CC=gcc 
CFLAGS=-Wall -std=c99 -lm

HEADERS = pgm/pgm.h contour/contour.h array/array.h correlation/correlation.h
OBJECTS = main.o pgm/pgm.o contour/contour.o array/array.o correlation/correlation.o

default: RUN

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

RUN: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f RUN

run: RUN
	./RUN

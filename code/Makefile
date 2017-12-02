CC=gcc 
CFLAGS=-Wall -std=c99 -lm

HEADERS = pgm/pgm.h isoline/isoline.h isoline/array.h
OBJECTS = main.o pgm/pgm.o isoline/isoline.o isoline/array.o

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
CC=gcc 
CFLAGS=-Wall -std=c99 -lm

HEADERS = pgm_io/pgm_io.h isoline/isoline.h
OBJECTS = main.o pgm_io/pgm_io.o isoline/isoline.o

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
CC=gcc 
CFLAGS=-Wall -std=c99 -lm

HEADERS = ../../main/pgm/pgm.h 
OBJECTS = countBlack.o ../../main/pgm/pgm.o 

default: COUNT

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

COUNT: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f COUNT

run: COUNT
	./COUNT
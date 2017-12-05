CC=gcc 
CFLAGS=-Wall

HEADERS = ../pgm/pgm.h
OBJECTS = rotate.o ../pgm/pgm.o

default: ROTATE

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

ROTATE: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f ROTATE
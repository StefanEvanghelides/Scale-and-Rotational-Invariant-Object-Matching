CC=gcc 
CFLAGS=-Wall

HEADERS = ../pgm/pgm.h
OBJECTS = convert.o ../pgm/pgm.o

default: CONVERT

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

CONVERT: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f CONVERT
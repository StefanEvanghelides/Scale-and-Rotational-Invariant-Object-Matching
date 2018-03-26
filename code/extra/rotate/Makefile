CC=gcc 
CFLAGS=-Wall

HEADERS = ../../main/pgm/pgm.h
OBJECTS = rotate.o ../../main/pgm/pgm.o

default: ROTATE

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

ROTATE: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f ROTATE
CC=gcc 
CFLAGS=-Wall -std=c99 -lm

HEADERS = ../../main/pgm/pgm.h 
OBJECTS = extract.o ../../main/pgm/pgm.o 

default: EXTRACT

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

EXTRACT: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f EXTRACT

run: EXTRACT
	./EXTRACT


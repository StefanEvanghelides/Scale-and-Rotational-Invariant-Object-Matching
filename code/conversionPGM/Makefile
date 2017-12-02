CC=gcc 
CFLAGS=-Wall

OBJECTS = convert.o

default: CONVERT

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

CONVERT: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f CONVERT
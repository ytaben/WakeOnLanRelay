CC=gcc
CFLAGS=

%.o: &.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
wolrelay: wolrelay.o
	$(CC) -o wolrelay wolrelay.o $(CFLAGS)

clean:
	rm -f *.o wolrelay


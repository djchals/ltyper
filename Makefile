CFLAGS=``
LIBS=-lm -lncurses -ljson-c

all:
	gcc -o typeminal main.c -Wall $(CFLAGS) $(LIBS) 
.PHONY: clean

clean:
	rm -f typeminal


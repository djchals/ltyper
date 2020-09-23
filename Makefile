CFLAGS=``
LIBS=-lm -lncurses -ljson-c

all:
	gcc -o ltyper main.c -Wall $(CFLAGS) $(LIBS) 
.PHONY: clean

clean:
	rm -f ltyper


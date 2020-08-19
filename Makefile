CFLAGS=`xml2-config --cflags`
LIBS=`xml2-config --libs` -lncurses

#en principio esto no hara falta pero lo guardo aquí por si acaso
#NCURSES_NO_UTF8_ACS=1

all: lectorxml.o
	gcc -o meca lectorxml.o main.c -Wall $(CFLAGS) $(LIBS)
lectorxml.o:
	gcc -c lectorxml.c -Wall $(CFLAGS) $(LIBS) 

.PHONY: clean

clean:
	rm -f meca


CFLAGS=`xml2-config --cflags`
LIBS=`xml2-config --libs` -lncurses

#en principio esto no hara falta pero lo guardo aquí por si acaso
#NCURSES_NO_UTF8_ACS=1

#all: lectorxml.o
#	gcc -o meca lectorxml.o main.c -Wall $(CFLAGS) $(LIBS)

#meca:
#	gcc -o meca main.c -Wall $(CFLAGS) $(LIBS) 
all:
	gcc -o meca main.c -Wall $(CFLAGS) $(LIBS) 
lectorxml.o:
	gcc -c lectorxml.c -Wall $(CFLAGS) $(LIBS) 

lectorxml:
	gcc -o lectorxml lectorxml.c $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f lectorxml.o

cleanall:
	rm -f lectorxml.o
	rm -f meca


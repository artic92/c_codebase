
all:  produttori_consumatori

produttori_consumatori: produttori_consumatori.o procedure.o
	gcc -o start produttori_consumatori.o procedure.o

produttori_consumatori.o:produttori_consumatori.c header.h
	gcc -c produttori_consumatori.c
   
procedure.o: procedure.c header.h
	gcc -c procedure.c
      
clean: 
	 rm -f *.o 
	rm -f ./start
	rm -f *.kdev*
	rm -f *~

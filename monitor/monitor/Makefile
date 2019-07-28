all: monitor.o semafori.o

monitor.o: monitor.h monitor.c
	gcc -c monitor.c

semafori.o: semafori.h semafori.c
	gcc -c semafori.c

clean:
	rm *.o
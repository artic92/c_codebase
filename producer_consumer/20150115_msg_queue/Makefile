all: driver client server

driver: driver.o
	gcc -o driver driver.o

client: client.o
	gcc -o client client.o
	
server: server.o semafori.o
	gcc -o server server.o semafori.o

driver.o: header.h driver.c
	gcc -c driver.c

client.o: header.h client.c 
	gcc -c client.c

server.o: header.h semafori.h server.c
	gcc -c server.c

semafori.o: semafori.h semafori.c
	gcc -c semafori.c

clean:
	rm *.o
	rm driver
	rm client
	rm server

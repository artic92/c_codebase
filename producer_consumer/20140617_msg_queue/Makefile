all: driver server client

driver: driver.o
	gcc -o driver driver.o
	
server: server.o
	gcc -o server server.o
	
client: client.o
	gcc -o client client.o

driver.o: header.h driver.c
	gcc -c driver.c
	
server.o: header.h server.c
	gcc -c server.c
	
client.o: header.h client.c
	gcc -c client.c
	
clean:
	rm *.o
	rm driver
	rm server
	rm client

all: driver server

server: server.o header.o
	gcc -lpthread -o server server.o header.o
	
driver: driver.o header.o
	gcc -o driver driver.o header.o
	
server.o: header.h server.c
	gcc -c server.c
	
driver.o: header.h driver.c
	gcc -c driver.c
	
header.o: header.h header.c
	gcc -c header.c
	
clean:
	rm *.o
	rm driver
	rm server

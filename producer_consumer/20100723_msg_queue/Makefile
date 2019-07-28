all: build start client server printer

build:
	mkdir -p build

build/driver.o: header.h driver.c
	gcc -c driver.c -o build/driver.o

build/client.o: header.h client.c
	gcc -c client.c -o build/client.o

build/server.o: header.h server.c
	gcc -c server.c -o build/server.o

build/printer.o: header.h printer.c
	gcc -c printer.c -o build/printer.o

start: build/driver.o
	gcc -o start build/driver.o -lpthread

client: build/client.o
	gcc -o client build/client.o -lpthread

server: build/server.o
	gcc -o server build/server.o -lpthread

printer: build/printer.o
	gcc -o printer build/printer.o -lpthread

clean:
	rm -rf build
	rm -f start client server printer

all: build start balancer client server1 server2 server3

build:
	mkdir -p build

build/driver.o: header.h driver.c
	gcc -c driver.c -o build/driver.o

build/balancer.o: header.h balancer.c
	gcc -c balancer.c -o build/balancer.o

build/client.o: header.h client.c
	gcc -c client.c -o build/client.o

build/server1.o: header.h server1.c
	gcc -c server1.c -o build/server1.o

build/server2.o: header.h server2.c
	gcc -c server2.c -o build/server2.o

build/server3.o: header.h server3.c
	gcc -c server3.c -o build/server3.o

start: build/driver.o
	gcc -o start build/driver.o

balancer: build/balancer.o
	gcc -o balancer build/balancer.o

client: build/client.o
	gcc -o client build/client.o

server1: build/server1.o
	gcc -o server1 build/server1.o

server2: build/server2.o
	gcc -o server2 build/server2.o

server3: build/server3.o
	gcc -o server3 build/server3.o

clean:
	rm -rf build/
	rm start
	rm balancer
	rm client
	rm server1
	rm server2
	rm server3


all: build chat

build:
	mkdir -p build

build/driver.o: header.h driver.c
	gcc -c driver.c -o build/driver.o

build/chat.o: header.h chat.c
	gcc -c chat.c -o build/chat.o

chat: build/driver.o build/chat.o
	gcc -o chat build/driver.o build/chat.o

clean:
	rm -rf build
	rm chat

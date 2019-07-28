OBJ = build/header.o build/driver.o

all: build driver

build:
	mkdir -p build

driver:  $(OBJ)
	gcc -o driver $(OBJ)

build/header.o: header.h header.c
	gcc -c header.c -o build/header.o

build/driver.o: header.h driver.c
	gcc -c driver.c -o build/driver.o

clean:
	rm -rf build
	rm driver

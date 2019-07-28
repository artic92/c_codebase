all: build start visualizzatore

build:
	mkdir -p build

start: build/driver.o build/header.o build/semafori.o
	gcc -o start build/driver.o build/header.o build/semafori.o

visualizzatore: build/visualizzatore.o
	gcc -o visualizzatore visualizzatore.c

build/header.o: header.h header.c
	gcc -c header.c -o $@

build/driver.o: header.h driver.c
	gcc -c driver.c -o $@

build/semafori.o: semafori.h semafori.c
	gcc -c semafori.c -o $@

build/visualizzatore.o: header.h visualizzatore.c
	gcc -c visualizzatore.c -o $@

clean:
	rm -rf build
	rm *.o
	rm start
	rm visualizzatore

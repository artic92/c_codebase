all: build driver studente docente

build:
	mkdir -p build

driver: build/driver.o
	gcc -o $@ $?

studente: build/studente.o build/semafori.o
	gcc -o $@ build/studente.o build/semafori.o

docente: docente.o build/semafori.o
	gcc -o $@ docente.o build/semafori.o

build/studente.o: header.h semafori.h studente.c
	gcc -c studente.c -o $@

build/docente.o: header.h semafori.h docente.c
	gcc -c docente.c -o $@

build/driver.o: header.h driver.c
	gcc -c driver.c -o $@

build/semafori.o: semafori.h semafori.c
	gcc -c semafori.c -o $@

clean:
	rm -rf build
	rm driver
	rm docente
	rm studente

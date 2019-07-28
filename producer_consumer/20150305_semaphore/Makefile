all: build driver produttore consumatore

build:
	mkdir -p build

driver: build/driver.o
	gcc -o driver build/driver.o

produttore: build/produttore.o build/semafori.o
	gcc -o produttore build/produttore.o build/semafori.o

consumatore: build/consumatore.o build/semafori.o
	gcc -o consumatore build/consumatore.o build/semafori.o

build/driver.o: header.h driver.c
	gcc -c driver.c -o build/driver.o

build/produttore.o: header.h semafori.h produttore.c
	gcc -c produttore.c -o build/produttore.o

build/consumatore.o: header.h semafori.h consumatore.c
	gcc -c consumatore.c -o build/consumatore.o

build/semafori.o: semafori.h semafori.c
	gcc -c semafori.c -o build/semafori.o

clean:
	rm -rf build
	rm driver
	rm consumatore
	rm produttore

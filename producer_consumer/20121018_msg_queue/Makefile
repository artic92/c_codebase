all: driver p1 p2 p3 p4 p5 p6

driver.o: header.h driver.c
	gcc -c driver.c
	
driver: driver.o
	gcc -o $@ $?
	
p1.o: header.h p1.c
	gcc -c p1.c
	
p1: p1.o
	gcc -o p1 p1.o
	
p2.o: header.h p2.c
	gcc -c p2.c
	
p2: p2.o
	gcc -o p2 p2.o
	
p3.o: header.h p3.c
	gcc -c p3.c
	
p3: p3.o
	gcc -o p3 p3.o
	
p4.o: header.h p4.c
	gcc -c p4.c
	
p4: p4.o
	gcc -o p4 p4.o
	
p5.o: header.h p5.c
	gcc -c p5.c
	
p5: p5.o
	gcc -o p5 p5.o
	
p6.o: header.h p6.c
	gcc -c p6.c
	
p6: p6.o
	gcc -o p6 p6.o
	
clean:
	rm *.o
	rm driver
	rm p1
	rm p2
	rm p3
	rm p4
	rm p5
	rm p6

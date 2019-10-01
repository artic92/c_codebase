CC = gcc
CFLAGS  += -Wall
CFLAGS  += -g
CFLAGS  += -fPIC

all: readsysfs readwritefile

%.o : %.c
	$(CC) -o $@ -c $(CFLAGS) $<

readsysfs: readsysfs.o
	@echo "Building $@"
	@$(CC) -o $@ $^

readwritefile: readwritefile.o
	@echo "Building $@"
	@$(CC) -o $@ $^

.PHONY = clean

clean:
	-rm -f readsysfs $(OBJS)
	-rm -f readwritefile
	-rm -f *.o *~

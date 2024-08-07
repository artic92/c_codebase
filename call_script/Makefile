CC = gcc
CFLAGS  += -Wall
CFLAGS  += -g
CFLAGS  += -fPIC

all: call_script

%.o : %.c
	$(CC) -o $@ -c $(CFLAGS) $<

call_script: call_script.o
	@echo "Building $@"
	@$(CC) -o $@ $^

.PHONY = clean

clean:
	-rm -f call_script $(OBJS)
	-rm -f *.o *~

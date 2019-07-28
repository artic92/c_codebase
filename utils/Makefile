IDIR		= .
ODIR		= obj
CC 			= gcc
CFLAGS		= -I$(IDIR) -g

_DEPS = utils.h utils.c test.c
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = utils.o test.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(ODIR) test

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(ODIR)
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~
	rm test

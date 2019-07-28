##########################################################################
# for x86
# make
#
# for imx
# make ARCH=imx
#
# for xilinx
# make ARCH=xilinx
##########################################################################
CROSS_COMPILE_IMX=arm-none-linux-gnueabi-
CROSS_COMPILE_XILINX=arm-linux-gnueabihf-

ifeq ($(ARCH),imx)
	COMPILER_PREFIX=$(CROSS_COMPILE_IMX)
else ifeq ($(ARCH),xilinx)
	COMPILER_PREFIX=$(CROSS_COMPILE_XILINX)
endif

IDIR				= .
CC					= gcc
CFLAGS				= -I$(IDIR) -g
CC   				= $(COMPILER_PREFIX)gcc

ODIR=obj

_OBJ = read_from_input_device.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(ODIR) read_from_input_device

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

read_from_input_device: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(ODIR)
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~
	rm read_from_input_device

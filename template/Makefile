##########################################################################
# for local platform
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

ifeq ($(ARCH),arm)
	COMPILER_PREFIX=$(CROSS_COMPILE_IMX)
else ifeq ($(ARCH),xilinx)
	COMPILER_PREFIX=$(CROSS_COMPILE_XILINX)
endif

IDIR		= ./include
ODIR		= obj
CC 			= $(COMPILER_PREFIX)gcc
CFLAGS		= -I$(IDIR) -g

# $(subst from,to,text): performs a textual replacement on the text 'text':
# each occurrence of 'from' is replaced by 'to'. The result is substituted for the function call.
# For example: $(subst ee,EE,feet on the street) substitutes the string ‘fEEt on the strEEt’.

_DEPS = hellomake.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = hellomake.o hellofunc.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(ODIR) hellomake

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	# 'core' are coredumps
	rm -rf $(ODIR)
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~
	rm hellomake

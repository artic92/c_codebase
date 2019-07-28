C = gcc
BUILD_DIR = ./build
OBJS = $(BUILD_DIR)/treni.o $(BUILD_DIR)/driver.o
FLG = -c

all: $(BUILD_DIR) start

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

start: $(OBJS)
	$(C) -o start $(OBJS)

$(BUILD_DIR)/treni.o: header.h treni.c | $(BUILD_DIR)
	$(C) $(FLG) treni.c -o $(BUILD_DIR)/treni.o

$(BUILD_DIR)/driver.o: driver.c header.h | $(BUILD_DIR)
	$(C) $(FLG) driver.c -o $(BUILD_DIR)/driver.o

clean:
	rm -rf $(BUILD_DIR)
	rm -f start

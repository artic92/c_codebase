C = gcc
BUILD_DIR = ./build
DIP = $(BUILD_DIR)/header.o $(BUILD_DIR)/driver.o
DIP1 = header.h header.c
DIP2 = header.h driver.c

ifdef DEBUG
FLG = -c -DDEBUG
else
FLG = -c
endif

all: $(BUILD_DIR) driver

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

driver: $(DIP)
	$(C) -o driver $(DIP)

$(BUILD_DIR)/driver.o: $(DIP2) | $(BUILD_DIR)
	$(C) $(FLG) driver.c -o $(BUILD_DIR)/driver.o

$(BUILD_DIR)/header.o: $(DIP1) | $(BUILD_DIR)
	$(C) $(FLG) header.c -o $(BUILD_DIR)/header.o

clean:
	rm -rf $(BUILD_DIR)
	rm -f driver

C = gcc
BUILD_DIR = ./build
OBJS = $(BUILD_DIR)/monitor.o $(BUILD_DIR)/header.o $(BUILD_DIR)/driver.o
FLG = -c

all: $(BUILD_DIR) start

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

start: $(OBJS)
	$(C) -o start $(OBJS)

$(BUILD_DIR)/monitor.o: monitor.h monitor.c | $(BUILD_DIR)
	$(C) $(FLG) monitor.c -o $(BUILD_DIR)/monitor.o

$(BUILD_DIR)/header.o: header.h monitor.h header.c | $(BUILD_DIR)
	$(C) $(FLG) header.c -o $(BUILD_DIR)/header.o

$(BUILD_DIR)/driver.o: driver.c header.h monitor.h | $(BUILD_DIR)
	$(C) $(FLG) driver.c -o $(BUILD_DIR)/driver.o

clean:
	rm -rf $(BUILD_DIR)
	rm -f start

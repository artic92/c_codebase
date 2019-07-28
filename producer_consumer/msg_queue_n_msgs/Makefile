BUILD_DIR = ./build
SRC_DIR = .

all: start p1 p2 p3

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

start: $(BUILD_DIR)/start.o $(BUILD_DIR)/header.o
	gcc -o start $(BUILD_DIR)/start.o $(BUILD_DIR)/header.o

p1: $(BUILD_DIR)/p1.o $(BUILD_DIR)/header.o
	gcc -o p1 $(BUILD_DIR)/p1.o $(BUILD_DIR)/header.o

p2: $(BUILD_DIR)/p2.o $(BUILD_DIR)/header.o
	gcc -o p2 $(BUILD_DIR)/p2.o $(BUILD_DIR)/header.o

p3: $(BUILD_DIR)/p3.o $(BUILD_DIR)/header.o
	gcc -o p3 $(BUILD_DIR)/p3.o $(BUILD_DIR)/header.o

$(BUILD_DIR)/header.o: $(SRC_DIR)/header.h $(SRC_DIR)/header.c | $(BUILD_DIR)
	gcc -c $(SRC_DIR)/header.c -o $(BUILD_DIR)/header.o

$(BUILD_DIR)/start.o: $(SRC_DIR)/header.h $(SRC_DIR)/start.c | $(BUILD_DIR)
	gcc -c $(SRC_DIR)/start.c -o $(BUILD_DIR)/start.o

$(BUILD_DIR)/p1.o: $(SRC_DIR)/header.h $(SRC_DIR)/p1.c | $(BUILD_DIR)
	gcc -c $(SRC_DIR)/p1.c -o $(BUILD_DIR)/p1.o

$(BUILD_DIR)/p2.o: $(SRC_DIR)/header.h $(SRC_DIR)/p2.c | $(BUILD_DIR)
	gcc -c $(SRC_DIR)/p2.c -o $(BUILD_DIR)/p2.o

$(BUILD_DIR)/p3.o: $(SRC_DIR)/header.h $(SRC_DIR)/p3.c | $(BUILD_DIR)
	gcc -c $(SRC_DIR)/p3.c -o $(BUILD_DIR)/p3.o

clean:
	rm -rf $(BUILD_DIR)
	rm -f start p1 p2 p3

CC = gcc
CFLAGS = -g -pthread -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
BUILD_PATH = build
SRC_DIR = src
OBJS = buffer.o primitives.o rollercoaster.o
PROGRAMS = rollercoaster.x rollercoasterlog.x

all: $(PROGRAMS)

rollercoasterlog.x:
	$(CC) $(CFLAGS) $(SRC_DIR)/rollercoasterlog.c -o rollercoasterlog.x

rollercoaster.x: $(OBJS)
	$(CC) $(CFLAGS) $(BUILD_PATH)/*.o -o rollercoaster.x

buffer.o: $(SRC_DIR)/buffer/buffer.c $(SRC_DIR)/buffer/buffer.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/buffer/buffer.c -o $(BUILD_PATH)/buffer.o

primitives.o: $(SRC_DIR)/primitives/primitives.c $(SRC_DIR)/primitives/primitives.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/primitives/primitives.c -o $(BUILD_PATH)/primitives.o

rollercoaster.o: $(SRC_DIR)/rollercoaster.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/rollercoaster.c -o $(BUILD_PATH)/rollercoaster.o

clean:
	rm -f *.o *~ $(PROGRAMS)
	rm -f $(BUILD_PATH)/*.o

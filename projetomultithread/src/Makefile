CC = gcc
CFLAGS = -g -pthread -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
BUILD_PATH = ../build

PROGRAMS = rollercoaster rollercoasterlog

all: $(PROGRAMS)

buffer.o: buffer/buffer.c buffer/buffer.h
	$(CC) $(CFLAGS) -c buffer/buffer.c -o $(BUILD_PATH)/buffer.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o $(BUILD_PATH)/main.o

main: main.o buffer.o
	$(CC) $(CFLAGS) $(BUILD_PATH)/*.o -o $(BUILD_PATH)/rollercoaster

clean:
	rm -f *.o *~ $(PROGRAMS)
	rm -f $(BUILD_PATH)/*.o

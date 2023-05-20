CC = gcc
CFLAGS = -g -pthread -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
BUILD_PATH = ../build
OBJS = buffer.o primitives.o rollercoaster.o
PROGRAMS = rollercoaster

all: $(PROGRAMS)

rollercoaster: $(OBJS)
	$(CC) $(CFLAGS) $(BUILD_PATH)/*.o -o rollercoaster

buffer.o: buffer/buffer.c buffer/buffer.h
	$(CC) $(CFLAGS) -c buffer/buffer.c -o $(BUILD_PATH)/buffer.o

primitives.o: primitives/primitives.c primitives/primitives.o
	$(CC) $(CFLAGS) -c primitives/primitives.c -o $(BUILD_PATH)/primitives.o

rollercoaster.o: rollercoaster.c
	$(CC) $(CFLAGS) -c rollercoaster.c -o $(BUILD_PATH)/rollercoaster.o

clean:
	rm -f *.o *~ $(PROGRAMS)
	rm -f $(BUILD_PATH)/*.o
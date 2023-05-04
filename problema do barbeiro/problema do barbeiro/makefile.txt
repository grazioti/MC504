CC = gcc
CFLAGS = -g -pthread -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

PROGRAMS = sinc_semaforo escr_leit barbeiro barbeiros barbearia

all: $(PROGRAMS)

clean:
	rm -f *.o *~ $(PROGRAMS)

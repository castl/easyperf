CFLAGS=-O2 -g -Wall 
RANLIB=ranlib

all: test libeasyperf.a

libeasyperf.a: easyperf.o
	$(AR) rc $@ $^
	$(RANLIB) $@

test: easyperf.o test.o
	$(CC) -o $@ $^ -lm

clean:
	rm -f *.o test libeasyperf.a

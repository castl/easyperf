CFLAGS=-O2 -g -Wall 
#-I/lib/modules/`uname -r`/include/build

all: test

test: easyperf.o test.o
	$(CC) -o $@ $^ -lm

clean:
	rm -f *.o test
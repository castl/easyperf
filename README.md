easyperf
========

An easy-to-use interface to the Linux perf_event API.

Example code:
```c
#include "easyperf.h"

#include <math.h>
#include <stdio.h>

volatile double d = 3.3;
void foo(unsigned long l) {
	unsigned long i;
	for (i=0; i<l; i++) {
		if (i & 1) {
			d = pow(d, 2);
		} else {
			d = sqrt(d);
		}
	}
}

int main(void) {
	uint64_t start[2], end[2];
	unsigned long i;
	perf_init(2, EV_CYCLES, EV_INSTR);


	for (i = 0; i<10; i++) {
		unsigned long loops = 1ul << (i + 10);
		perf_read_all(start);

		foo(loops);

		perf_read_all(end);

		printf("[%6lu] Cycles: %10lu, Instrs: %10lu\n",
				loops, end[0] - start[0], end[1] - start[1]);
	}

	perf_close();
	return 0;
}
```


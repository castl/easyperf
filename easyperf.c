#define _GNU_SOURCE


#include <assert.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "easyperf.h"

//perf counter syscall
static inline int perf_event_open(struct perf_event_attr * hw,
			    pid_t pid, int cpu, int grp, unsigned long flags)
{
    return syscall(__NR_perf_event_open, hw, pid, cpu, grp, flags);
}

static struct perf_event_attr *attr = NULL;
static int *fds = NULL;
static unsigned int number;

int perf_init(unsigned int num_ctrs, ...) {
    va_list args;
	int ctr, ret = 0;
	number = num_ctrs;

	attr = (struct perf_event_attr*)calloc(num_ctrs, sizeof(struct perf_event_attr));
	fds  = (int*) malloc(sizeof(int) * num_ctrs);
	if (attr == NULL || fds == NULL) {
		perror("Could not allocate space for counter data: ");
		ret = -1;
		goto error;
	}

    //Configure each counter, staring with 1
    va_start(args, num_ctrs);
    for (ctr=0; ctr<num_ctrs; ctr++) {
        unsigned event, conf;
        conf = va_arg(args, unsigned int) & 0xFFFFFF;
        event = conf & 0xFFFF;

		attr[ctr].type = PERF_TYPE_RAW;
		attr[ctr].config = event; /* generic PMU event*/
		attr[ctr].disabled = 0;
		attr[ctr].pinned = 1;
		attr[ctr].inherit = 1;
		fds[ctr] = perf_event_open(&attr[ctr], getpid(), -1, -1, 0);
		if ((conf & PERFMON_EVENTSEL_OS) == 0)
			attr[ctr].exclude_kernel = 1; 
		if ((conf & PERFMON_EVENTSEL_USR) == 0)
			attr[ctr].exclude_user= 1; 

		if (fds[ctr] < 0) {
			perror("Opening performance counter");
			goto error;
		}
    }
    va_end(args);

    return ret;

error:
	number = 0;
	if (attr) {
		free(attr);
		attr = NULL;
	}
	if (fds) {
		free(fds);
		fds = NULL;
	}
	return ret;
}

void perf_close() {
	unsigned int i;
	for (i=0; i<number; i++) {
		close(fds[i]);
	}

	number = 0;
	if (attr) {
		free(attr);
		attr = NULL;
	}
	if (fds) {
		free(fds);
		fds = NULL;
	}
}

uint64_t perf_read(unsigned int ctr) {
	uint64_t val;
	int rc;
	assert(ctr < number);
	rc = read(fds[ctr], &val, sizeof(val));
	assert(rc == sizeof(val));
	return val;
}

void perf_read_all(uint64_t* vals) {
	unsigned int i;
	for (i=0; i<number; i++) {
		vals[i] = perf_read(i);
	}
}

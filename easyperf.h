#ifndef __EASYPERF_H__
#define __EASYPERF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Extra options for each event
#define PERFMON_EVENTSEL_OS     (1 << 17)
#define PERFMON_EVENTSEL_USR    (1 << 16)

int perf_init(unsigned int num_ctrs, ...);
void perf_close();

uint64_t perf_read(unsigned int ctr);
void perf_read_all(uint64_t* vals);

// microarch neutral
#define EV_CYCLES          (0x3C | (0x0 << 8))
#define EV_REF_CYCLES      (0x3C | (0x1 << 8))
#define EV_INSTR           (0xC0 | (0x0 << 8))
#define EV_BRANCH          (0xC4 | (0x1 << 8))
#define EV_BRANCH_MISS     (0xC5 | (0x1 << 8))

// microarch specific
#define I7_L3_REFS      (0x2e | (0x4f << 8))
#define I7_L3_MISS      (0x2e | (0x41 << 8))

#define I7_L2_REFS      (0x24 | (0xff << 8))
#define I7_L2_MISS      (0x24 | (0xaa << 8))

#define I7_ICACHE_HITS  (0x80 | (0x01 << 8))
#define I7_ICACHE_MISS  (0x80 | (0x02 << 8))

#define I7_DL1_REFS     (0x43 | (0x01 << 8))

#define I7_LOADS        (0x0b | (0x01 << 8))
#define I7_STORES       (0x0b | (0x02 << 8))

#define I7_L2_DTLB_MISS (0x49 | (0x01 << 8))
#define I7_L2_ITLB_MISS (0x85 | (0x01 << 8))

#define I7_IO_TXNS      (0x6c | (0x01 << 8))
#define I7_DRAM_REFS    (0x0f | (0x20 << 8))

#ifdef __cplusplus
}
#endif

#endif // __EASYPERF_H__
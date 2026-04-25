#ifndef CPU_H
#define CPU_H

#define MAX_CORES 20

typedef struct {
  char label[16];
  long user;
  long nice;
  long system;
  long idle;
  long iowait;
  long irq;
  long softirq;
  long steal;
} CpuStat;
int read_cpu_stats(CpuStat *stats, int max_cores);

double calc_usage(CpuStat *prev, CpuStat *curr);

#endif

#include "cpu.h"
#include <stdio.h>
#include <string.h>
int read_cpu_stats(CpuStat *stats, int max_cores) {
  FILE *fp = fopen("/proc/stat", "r");
  if (!fp) {
    perror("fopen /proc/stat");
    return -1;
  }

  int count = 0;
  char line[256];
  while (fgets(line, sizeof(line), fp) && count < max_cores) {
    if (strncmp(line, "cpu", 3) != 0)
      break;
    CpuStat *s = &stats[count];
    sscanf(line, "%s %ld %ld %ld %ld %ld %ld %ld %ld", s->label, &s->user,
           &s->nice, &s->system, &s->idle, &s->iowait, &s->irq, &s->softirq,
           &s->steal);
    count++;
  }
  fclose(fp);
  return count;
}
long get_total(CpuStat *s) {
  return s->user + s->nice + s->system + s->idle + s->iowait + s->irq +
         s->softirq + s->steal;
}
long get_idle(CpuStat *s) { return s->idle + s->iowait; }
double calc_usage(CpuStat *prev, CpuStat *curr) {
  long delta_total = get_total(curr) - get_total(prev);
  long delta_idle = get_idle(curr) - get_idle(prev);
  if (delta_total == 0)
    return 0.0;
  return (1.0 - (double)delta_idle / delta_total) * 100.0;
}

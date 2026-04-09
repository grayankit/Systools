#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

long read_cpu_stats(CpuStat *stats, int max_cores) {
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

int main() {
  CpuStat prev[MAX_CORES], curr[MAX_CORES];
  int num_cpus;

  num_cpus = read_cpu_stats(prev, MAX_CORES);
  if (num_cpus < 1)
    return 1;
  while (1) {
    sleep(1);
    num_cpus = read_cpu_stats(curr, MAX_CORES);
    printf("\033[2J\033[H");
    for (int i = 0; i<num_cpus; i++) {
      double usage = calc_usage(&prev[i], &curr[i]);
      printf("%-6s[",curr[i].label);
      int bars = (int)(usage / 5);
      for (int b = 0; b<20; b++) {
        putchar(b<bars?'o':' ');
      }
      printf("] %5.1f%%\n",usage);
    }
  memcpy(prev, curr,sizeof(CpuStat)*num_cpus);
  }
return 0;
}

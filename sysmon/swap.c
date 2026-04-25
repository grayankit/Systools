#include "swap.h"
#include <stdio.h>

long read_swaps_info(Swapstats *info, int max_swaps) {
  FILE *fp = fopen("/proc/swaps", "r");
  if (!fp) {
    perror("fopen /proc/swaps");
    return -1;
  }
  char line[256];
  int count = 0;
  fgets(line, sizeof(line), fp);
  while (fgets(line, sizeof(line), fp) && count < max_swaps) {
    Swapstats *s = &info[count];
    sscanf(line, "%63s %14s %lu %lu %d", s->FileName, s->Type, &s->size_kb,
           &s->used_kb, &s->priority);
    count++;
  }
  fclose(fp);
  return count;
}

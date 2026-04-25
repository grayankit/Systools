#include "memory.h"
#include <stdio.h>

int read_memory_info(MemInfo *info) {
  FILE *fp = fopen("/proc/meminfo", "r");
  if (!fp) {
    perror("fopen /proc/meminfo");
    return -1;
  }
  char line[256];
  while (fgets(line, sizeof(line), fp)) {
    if (sscanf(line, "MemTotal: %lu kB", &info->total_kb) == 1)
      continue;
    if (sscanf(line, "MemFree: %lu kB", &info->free_kb) == 1)
      continue;
    if (sscanf(line, "MemAvailable: %lu kB", &info->available_kb) == 1)
      continue;
    if (sscanf(line, "Buffers: %lu kB", &info->buffers_kb) == 1)
      continue;
    if (sscanf(line, "Cached: %lu kB", &info->cached_kb) == 1)
      continue;
    if (sscanf(line, "SwapTotal: %lu kB", &info->swap_total_kb) == 1)
      continue;
    if (sscanf(line, "SwapFree: %lu kB", &info->swap_free_kb) == 1)
      continue;
  }
  fclose(fp);
  return 0;
}

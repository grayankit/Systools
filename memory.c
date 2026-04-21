#include <stdio.h>
#include <unistd.h>

typedef struct {
  unsigned long total_kb;
  unsigned long free_kb;
  unsigned long available_kb;
  unsigned long buffers_kb;
  unsigned long cached_kb;
  unsigned long swap_total_kb;
  unsigned long swap_free_kb;
} MemInfo;

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
int main(void) {
  MemInfo info;
  while (1) {
    if (read_memory_info(&info) < 0)
      return 1;
    unsigned long used = info.total_kb - info.available_kb;
    unsigned long swap_used = info.swap_total_kb - info.swap_free_kb;
    double mem_usage_pct = (double)used / info.total_kb * 100.0;
    double swap_usage_pct = (double)swap_used / info.swap_total_kb * 100.0;

    printf("\033[2J\033[H");
    printf("RAM: %.1lfGB / %.1lfGB (%.1lf%%)\n", used / (1024.0 * 1024.0),
           info.total_kb / (1024.0 * 1024.0), mem_usage_pct);
    printf("SWAP: %.1lfGB / %.1lfGB (%.1lf%% )\n",
           swap_used / (1024.0 * 1024.0),
           info.swap_total_kb / (1024.0 * 1024.0), swap_usage_pct);
    sleep(1);
  }
}

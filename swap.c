

#include <stdio.h>
#include <unistd.h>

#define MAX_SWAPS 5

typedef struct {
  char FileName[100];
  char Type[15];
  unsigned long size_kb;
  unsigned long used_kb;
  int priority;
} Swaps;

long read_swaps_info(Swaps *info, int max_swaps) {
  FILE *fp = fopen("/proc/swaps", "r");
  if (!fp) {
    perror("fopen /proc/swaps");
    return -1;
  }
  char line[256];
  int count = 0;
  fgets(line, sizeof(line), fp);
  while (fgets(line, sizeof(line), fp) && count < max_swaps) {
    Swaps *s = &info[count];
    sscanf(line, "%63s %14s %lu %lu %d", s->FileName, s->Type, &s->size_kb,
           &s->used_kb, &s->priority);
    count++;
  }
  fclose(fp);
  return count;
}
void print_swap_info(Swaps *stats, int count) {
  printf("\033[2J\033[H");
  printf("%-35s %-12s %10s %10s %10s %8s\n", "Device", "Type", "Size", "Used",
         "Free", "Priority");
  printf("---------------------------------------------------------------------"
         "-\n");
  for (int i = 0; i < count; i++) {
    Swaps *s = &stats[i];

    unsigned long free_kb = s->size_kb - s->used_kb;
    double usage_per = (double)s->used_kb / s->size_kb * 100.0;
    double size_mb = s->size_kb / 1024.0;
    double used_mb = s->used_kb / 1024.0;
    double free_mb = free_kb / 1024.0;

    int bars = (int)(usage_per / 5);
    char bar[21];
    for (int b = 0; b < 20; b++) {
      bar[b] = b < bars ? '#' : ' ';
    }
    bar[20] = '\0';
    printf("%-35s %-12s %8.1fMB %8.1fMB %8.1fMB %8d\n", s->FileName, s->Type,
           size_mb, used_mb, free_mb, s->priority);
    printf("  [%s] %.1f%%\n", bar, usage_per);
  }
}
int main() {
  Swaps stats[MAX_SWAPS];

  while (1) {
    int count = read_swaps_info(stats, MAX_SWAPS);
    if (count < 0)
      return 1;
    print_swap_info(stats, count);
    sleep(2);
  }
  return 0;
}

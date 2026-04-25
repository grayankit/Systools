#include "display.h"
#include "swap.h"
#include <stdio.h>

static void format_speed(unsigned long bytes_per_sec, char *buf, int bufsize) {
  if (bytes_per_sec >= 1024 * 1024)
    snprintf(buf, bufsize, "%6.1f MB/s", bytes_per_sec / (1024.0 * 1024.0));
  else
    snprintf(buf, bufsize, "%6.1f KB/s", bytes_per_sec / 1024.0);
}
void display_uptime(Uptime *u) {
  int days = (int)u->uptime_seconds / 86400;
  int hours = ((int)u->uptime_seconds % 86400) / 3600;
  int minutes = ((int)u->uptime_seconds % 3600) / 60;
  int secs = (int)u->uptime_seconds % 60;

  printf("UPTIME\n");
  printf("--------------------------------------------------\n");
  printf("%d days, %02d:%02d:%02d\n\n", days, hours, minutes, secs);
}

void display_memory(MemInfo *info) {
  unsigned long used = info->total_kb - info->available_kb;
  unsigned long swap_used = info->swap_total_kb - info->swap_free_kb;

  double mem_usage_pct = (double)used / info->total_kb * 100.0;
  double swap_usage_pct = (double)swap_used / info->swap_total_kb * 100.0;

  int mem_bars = (int)(mem_usage_pct / 5);
  int swap_bars = (int)(swap_usage_pct / 5);

  printf("MEMORY\n");
  printf("--------------------------------------------------\n");

  printf("RAM:  ");
  for (int b = 0; b < 20; b++)
    putchar(b < mem_bars ? '#' : ' ');
  printf("  %.1fGB / %.1fGB (%.1f%%)\n", used / (1024.0 * 1024.0),
         info->total_kb / (1024.0 * 1024.0), mem_usage_pct);

  printf("SWAP: ");
  for (int b = 0; b < 20; b++)
    putchar(b < swap_bars ? '#' : ' ');
  printf("  %.1fGB / %.1fGB (%.1f%%)\n\n", swap_used / (1024.0 * 1024.0),
         info->swap_total_kb / (1024.0 * 1024.0), swap_usage_pct);
}

void display_cpu(CpuStat *prev, CpuStat *curr, int num_cpus) {
  printf("CPU\n");
  printf("--------------------------------------------------\n");
  for (int i = 0; i < num_cpus; i++) {
    double usage = calc_usage(&prev[i], &curr[i]);
    printf("%-6s[", curr[i].label);
    int bars = (int)(usage / 5);
    for (int b = 0; b < 20; b++) {
      putchar(b < bars ? 'o' : ' ');
    }
    printf("] %5.1f%%\n", usage);
  }
}
void display_net(NetStat *prev, NetStat *curr, int num_ifaces) {
  printf("NETWORK\n");
  printf("--------------------------------------------------\n");

  printf("%-20s %15s %15s %10s %10s\n", "Interface", "RX", "TX", "RX drop",
         "TX drop");
  printf("--------------------------------------------------------------\n");
  for (int i = 0; i < num_ifaces; i++) {
    unsigned long rx_speed, tx_speed;
    calc_speed(&prev[i], &curr[i], 1, &rx_speed, &tx_speed);
    char rx_buf[32], tx_buf[32];
    format_speed(rx_speed, rx_buf, sizeof(rx_buf));
    format_speed(tx_speed, tx_buf, sizeof(tx_buf));

    printf("%-20s %15s %15s %10lu %10lu\n", curr[i].name, rx_buf, tx_buf,
           curr[i].rx_drop, curr[i].tx_drop);
  }
}
void display_swap(Swapstats *stats, int count) {
  printf("SWAPS\n");
  printf("--------------------------------------------------\n");
  printf("%-35s %-12s %10s %10s %10s %8s\n", "Device", "Type", "Size", "Used",
         "Free", "Priority");
  printf("---------------------------------------------------------------------"
         "-\n");
  for (int i = 0; i < count; i++) {
    Swapstats *s = &stats[i];

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

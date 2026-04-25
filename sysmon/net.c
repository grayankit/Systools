#include "net.h"
#include <stdio.h>

int read_network_stats(NetStat *stats, int max_ifaces) {
  FILE *fp = fopen("/proc/net/dev", "r");
  if (!fp) {
    perror("fopen /proc/net/dev");
    return -1;
  }
  char line[256];
  int count = 0;
  fgets(line, sizeof(line), fp);
  fgets(line, sizeof(line), fp);

  while (fgets(line, sizeof(line), fp) && count < max_ifaces) {
    NetStat *s = &stats[count];
    sscanf(line, "%31[^:]: %lu %lu %*d %lu %*d %*d %*d %*d %lu %lu %*d %lu",
           s->name, &s->rx_bytes, &s->rx_packets, &s->rx_drop, &s->tx_bytes,
           &s->tx_packets, &s->tx_drop);
    count++;
  }
  fclose(fp);
  return count;
}
void calc_speed(NetStat *prev, NetStat *curr, int interval_sec,
                unsigned long *rx_speed, unsigned long *tx_speed) {
  *rx_speed = (curr->rx_bytes - prev->rx_bytes) / interval_sec;
  *tx_speed = (curr->tx_bytes - prev->tx_bytes) / interval_sec;
}

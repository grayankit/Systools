
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_IFACES 10
#define INTERVAL 1

typedef struct {
  char name[32];
  unsigned long rx_bytes;
  unsigned long rx_packets;
  unsigned long rx_drop;
  unsigned long tx_bytes;
  unsigned long tx_packets;
  unsigned long tx_drop;
} NetStat;

long read_network_stats(NetStat *stats, int max_ifaces) {
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
void format_speed(long bytes_per_sec, char *buf, int bufsize) {
  if (bytes_per_sec >= 1024 * 1024) {
    snprintf(buf, bufsize, "%6.1f MB/s", bytes_per_sec / (1024.0 * 1024.0));
  } else {

    snprintf(buf, bufsize, "%6.1f KB/s", bytes_per_sec / (1024.0));
  }
}
void calc_speed(NetStat *prev, NetStat *curr, int interval_sec, long *rx_speed,
                long *tx_speed) {
  *rx_speed = (curr->rx_bytes - prev->rx_bytes) / interval_sec;
  *tx_speed = (curr->tx_bytes - prev->tx_bytes) / interval_sec;
}

int main() {
  NetStat prev[MAX_IFACES], curr[MAX_IFACES];
  int num_ifaces;
  num_ifaces = read_network_stats(prev, MAX_IFACES);
  if (num_ifaces < 1) {
    return 1;
  }
  while (1) {
    sleep(INTERVAL);
    num_ifaces = read_network_stats(curr, MAX_IFACES);
    if (num_ifaces < 0) {
      break;
    }
    printf("\033[2J\033[H");
    printf("%-20s %15s %15s %10s %10s\n", "Interface", "RX", "TX", "RX drop", "TX drop");
    printf("--------------------------------------------------------------\n");
    for (int i = 0; i < num_ifaces; i++) {
      long rx_speed, tx_speed;
      calc_speed(&prev[i], &curr[i], INTERVAL, &rx_speed, &tx_speed);
      char rx_buf[32], tx_buf[32];
      format_speed(rx_speed, rx_buf, sizeof(rx_buf));
      format_speed(tx_speed, tx_buf, sizeof(tx_buf));

      printf("%-20s %15s %15s %10ld %10ld\n", curr[i].name, rx_buf, tx_buf,
             curr[i].rx_drop, curr[i].tx_drop);
    }
    memcpy(prev, curr, sizeof(NetStat) * num_ifaces);
  }
  return 0;
}

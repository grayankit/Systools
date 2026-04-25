#ifndef NET_H
#define NET_H

#define MAX_IFACES 10
typedef struct {
  char name[32];
  unsigned long rx_bytes;
  unsigned long rx_packets;
  unsigned long rx_drop;
  unsigned long tx_bytes;
  unsigned long tx_packets;
  unsigned long tx_drop;
} NetStat;

int read_network_stats(NetStat *stats, int max_ifaces);

void calc_speed(NetStat *prev, NetStat *curr, int interval_sec,
                unsigned long *rx_speed, unsigned long *tx_speed);
#endif // !NET_H

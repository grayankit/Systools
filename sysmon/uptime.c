#include "uptime.h"
#include <stdio.h>

int read_uptime(Uptime *u) {

  FILE *fp = fopen("/proc/uptime", "r");
  if (!fp) {
    perror("fopen /proc/uptime");
    return -1;
  }
  char line[256];
  fgets(line, sizeof(line), fp);
  sscanf(line, "%lf %lf", &u->uptime_seconds, &u->idle_seconds);
  fclose(fp);
  return 0;
}

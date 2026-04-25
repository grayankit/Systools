#ifndef UPTIME_H
#define UPTIME_H

typedef struct {
  double uptime_seconds;
  double idle_seconds;
} Uptime;

int read_uptime(Uptime *u);

#endif // !UPTIME_H


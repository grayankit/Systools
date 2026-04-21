#include <stdio.h>
#include <unistd.h>
typedef struct {
  double uptime_seconds;
  double idle_seconds;
} Uptime;
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
int main() {
  Uptime u;
  while (1) {
    if (read_uptime(&u) < 0)
      return 1;
    int days = (int)u.uptime_seconds / 86400;
    int hours = ((int)u.uptime_seconds % 86400) / 3600;
    int minutes = ((int)u.uptime_seconds % 3600) / 60;
    int seconds = (int)u.uptime_seconds % 60;
    printf("\033[2J\033[H");
    printf("Uptime: %d days, %02d hours, %02d minutes, %02d seconds\n", days, hours,
           minutes, seconds);
    // NOTE: idle can be used using u.idle_seconds
    //
    sleep(1);
  }
  return 0;
}

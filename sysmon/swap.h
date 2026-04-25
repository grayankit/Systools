#ifndef SWAP_H

#define SWAP_H

#define MAX_SWAPS 5

typedef struct {
  char FileName[100];
  char Type[15];
  unsigned long size_kb;
  unsigned long used_kb;
  int priority;
} Swapstats;
long read_swaps_info(Swapstats *info, int max_swaps);
#endif // !SWAP.H

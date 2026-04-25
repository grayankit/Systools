#ifndef DISPLAY_H
#define DISPLAY_H

#include "cpu.h"
#include "memory.h"
#include "uptime.h"
#include "net.h"
#include "swap.h"
void display_cpu(CpuStat *prev, CpuStat *curr, int num_cpus);
void display_memory(MemInfo *info);
void display_uptime(Uptime *uptime);
void display_net(NetStat *prev,NetStat *curr,int num_ifaces);
void display_swap(Swapstats *stats,int count);

#endif // !DISPLAY_H

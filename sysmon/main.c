#include "display.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    // CPU
    CpuStat cpu_prev[MAX_CORES], cpu_curr[MAX_CORES];
    int num_cpus = read_cpu_stats(cpu_prev, MAX_CORES);
    if (num_cpus < 0) return 1;

    // Network
    NetStat net_prev[MAX_IFACES], net_curr[MAX_IFACES];
    int num_ifaces = read_network_stats(net_prev, MAX_IFACES);
    if (num_ifaces < 0) return 1;

    // Memory, swap, uptime (no prev needed)
    MemInfo  mem;
    Swapstats swaps[MAX_SWAPS];
    Uptime   uptime;

    while (1) {
        sleep(1);

        // read all data
        num_cpus   = read_cpu_stats(cpu_curr, MAX_CORES);
        num_ifaces = read_network_stats(net_curr, MAX_IFACES);
        read_memory_info(&mem);
        int num_swaps = read_swaps_info(swaps, MAX_SWAPS);
        read_uptime(&uptime);

        // clear screen
        printf("\033[2J\033[H");
        printf("========== SYSTEM MONITOR ==========\n\n");

        // display all
        display_uptime(&uptime);
        display_cpu(cpu_prev, cpu_curr, num_cpus);
        display_memory(&mem);
        display_net(net_prev, net_curr, num_ifaces);
        display_swap(swaps, num_swaps);

        // update prev snapshots
        memcpy(cpu_prev, cpu_curr, sizeof(CpuStat) * num_cpus);
        memcpy(net_prev, net_curr, sizeof(NetStat) * num_ifaces);
    }

    return 0;
}

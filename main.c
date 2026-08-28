#include <stdio.h>
#include <string.h>

#include "system_info.h"

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            printf("Usage: %s [option]\n", argv[0]);
            printf("\n");
            printf("Options:\n");
            printf("  --help       Show this help message\n");
            printf("  --version    Show program version\n");

            return 0;
        }

        if (strcmp(argv[1], "--version") == 0)
        {
            printf("sys-monitor version 1.0\n");

            return 0;
        }

        printf("Error: unknown option '%s'\n", argv[1]);
        printf("Use '%s --help' for usage information.\n", argv[0]);

        return 1;
    }

    SystemInfo info;

    info.cpu_usage = get_cpu_usage();
    info.memory_usage = get_memory_usage();
    info.cpu_cores = get_cpu_cores();

    get_uptime(
        info.uptime,
        sizeof(info.uptime)
    );

    get_load_average(
        &info.load1,
        &info.load5,
        &info.load15
    );

    if (info.cpu_usage < 0)
    {
        printf("Error: cannot read CPU information\n");
        return 1;
    }

    if (info.memory_usage < 0)
    {
        printf("Error: cannot read memory information\n");
        return 1;
    }

    if (info.cpu_cores < 0)
    {
        printf("Error: cannot read CPU core information\n");
        return 1;
    }

    if (info.load1 < 0)
    {
        printf("Error: cannot read load average\n");
        return 1;
    }

    printf("\n");
    printf("================================\n");
    printf("          SYSTEM MONITOR\n");
    printf("================================\n");

    printf("CPU Usage:       %6.1f %%\n", info.cpu_usage);
    printf("Memory Usage:    %6.1f %%\n", info.memory_usage);
    printf("CPU Cores:       %6d\n", info.cpu_cores);

    printf(
        "Load Average:    %.2f %.2f %.2f\n",
        info.load1,
        info.load5,
        info.load15
    );

    printf("Uptime:          %s\n", info.uptime);

    printf("================================\n");

    return 0;
}

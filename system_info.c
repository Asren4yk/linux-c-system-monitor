#include "system_info.h"

#include <string.h>

#include <stdio.h>
#include <unistd.h>

double get_cpu_usage(void)
{
    FILE *file;
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;

    unsigned long long user2;
    unsigned long long nice2;
    unsigned long long system2;
    unsigned long long idle2;

    file = fopen("/proc/stat", "r");

    if (file == NULL)
    {
        return -1.0;
    }

    fscanf(
        file,
        "cpu %llu %llu %llu %llu",
        &user,
        &nice,
        &system,
        &idle
    );

    fclose(file);

    sleep(1);

    file = fopen("/proc/stat", "r");

    if (file == NULL)
    {
        return -1.0;
    }

    fscanf(
        file,
        "cpu %llu %llu %llu %llu",
        &user2,
        &nice2,
        &system2,
        &idle2
    );

    fclose(file);

    unsigned long long idle_delta = idle2 - idle;
    unsigned long long user_delta = user2 - user;
    unsigned long long nice_delta = nice2 - nice;
    unsigned long long system_delta = system2 - system;

    unsigned long long total =
        idle_delta +
        user_delta +
        nice_delta +
        system_delta;

    if (total == 0)
    {
        return 0.0;
    }

    unsigned long long busy =
        user_delta +
        nice_delta +
        system_delta;

    return (double)busy / total * 100.0;
}


double get_memory_usage(void)
{
    FILE *file;
    char label[64];

    unsigned long long total;
    unsigned long long available;

    file = fopen("/proc/meminfo", "r");

    if (file == NULL)
    {
        return -1.0;
    }

    total = 0;
    available = 0;

    while (fscanf(file, "%63s %llu kB", label, &total) == 2)
    {
        if (strcmp(label, "MemTotal:") == 0)
        {
            break;
        }
    }

    rewind(file);

    while (fscanf(file, "%63s %llu kB", label, &available) == 2)
    {
        if (strcmp(label, "MemAvailable:") == 0)
        {
            break;
        }
    }

    fclose(file);

    if (total == 0)
    {
        return -1.0;
    }

    return (double)(total - available) / total * 100.0;
}

void get_uptime(char *buffer, int size)
{
    FILE *file;
    double uptime_seconds;

    file = fopen("/proc/uptime", "r");

    if (file == NULL)
    {
        snprintf(buffer, size, "Unknown");
        return;
    }

    if (fscanf(file, "%lf", &uptime_seconds) != 1)
    {
        fclose(file);
        snprintf(buffer, size, "Unknown");
        return;
    }

    fclose(file);

    int total_seconds = (int)uptime_seconds;

    int days = total_seconds / 86400;
    total_seconds %= 86400;

    int hours = total_seconds / 3600;
    total_seconds %= 3600;

    int minutes = total_seconds / 60;
    int seconds = total_seconds % 60;

    if (days > 0)
    {
        snprintf(
            buffer,
            size,
            "%dd %02dh %02dm %02ds",
            days,
            hours,
            minutes,
            seconds
        );
    }
    else
    {
        snprintf(
            buffer,
            size,
            "%02dh %02dm %02ds",
            hours,
            minutes,
            seconds
        );
    }
}


void get_load_average(
    double *load1,
    double *load5,
    double *load15
)
{
    FILE *file;

    file = fopen("/proc/loadavg", "r");

    if (file == NULL)
    {
        *load1 = -1.0;
        *load5 = -1.0;
        *load15 = -1.0;
        return;
    }

    if (fscanf(
            file,
            "%lf %lf %lf",
            load1,
            load5,
            load15
        ) != 3)
    {
        *load1 = -1.0;
        *load5 = -1.0;
        *load15 = -1.0;
    }

    fclose(file);
}

int get_cpu_cores(void)
{
    FILE *file;
    char line[256];
    int cores = 0;

    file = fopen("/proc/cpuinfo", "r");

    if (file == NULL)
    {
        return -1;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "processor", 9) == 0)
        {
            cores++;
        }
    }

    fclose(file);

    return cores;
}

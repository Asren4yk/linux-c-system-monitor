#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#define UPTIME_BUFFER_SIZE 64

typedef struct
{
    double cpu_usage;
    double memory_usage;

int cpu_cores;

    double load1;
    double load5;
    double load15;

    char uptime[UPTIME_BUFFER_SIZE];

} SystemInfo;

double get_cpu_usage(void);
double get_memory_usage(void);

int get_cpu_cores(void);

void get_uptime(char *buffer, int size);

void get_load_average(
    double *load1,
    double *load5,
    double *load15
);

#endif

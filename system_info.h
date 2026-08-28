#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

typedef struct
{
    double cpu_usage;

    double memory_usage;
    double memory_total;
    double memory_used;
    double memory_free;

    int cpu_cores;

    double disk_usage;
    double disk_total;
    double disk_used;
    double disk_available;

    double load1;
    double load5;
    double load15;

    char uptime[64];

    char hostname[128];
    char os_name[128];
    char kernel_version[128];

} SystemInfo;


/* CPU */
double get_cpu_usage(void);


/* Memory */
double get_memory_usage(void);
double get_memory_total(void);
double get_memory_used(void);
double get_memory_free(void);


/* CPU cores */
int get_cpu_cores(void);


/* Uptime */
void get_uptime(char *buffer, int size);


/* Load average */
void get_load_average(
    double *load1,
    double *load5,
    double *load15
);


/* Disk */
double get_disk_usage(void);
double get_disk_total(void);
double get_disk_used(void);
double get_disk_available(void);


/* System */
int get_hostname(char *buffer, int size);
int get_os_name(char *buffer, int size);
int get_kernel_version(char *buffer, int size);

#endif

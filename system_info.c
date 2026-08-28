#include "system_info.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/statvfs.h>


/* =========================
   CPU
   ========================= */

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

    if (fscanf(
            file,
            "cpu %llu %llu %llu %llu",
            &user,
            &nice,
            &system,
            &idle
        ) != 4)
    {
        fclose(file);
        return -1.0;
    }

    fclose(file);

    sleep(1);

    file = fopen("/proc/stat", "r");

    if (file == NULL)
    {
        return -1.0;
    }

    if (fscanf(
            file,
            "cpu %llu %llu %llu %llu",
            &user2,
            &nice2,
            &system2,
            &idle2
        ) != 4)
    {
        fclose(file);
        return -1.0;
    }

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


/* =========================
   Memory
   ========================= */

double get_memory_usage(void)
{
    double total = get_memory_total();
    double free_memory = get_memory_free();

    if (total < 0 || free_memory < 0)
    {
        return -1.0;
    }

    return (total - free_memory) / total * 100.0;
}


double get_memory_total(void)
{
    FILE *file;
    char label[64];
    unsigned long long total;

    file = fopen("/proc/meminfo", "r");

    if (file == NULL)
    {
        return -1.0;
    }

    while (fscanf(file, "%63s %llu kB", label, &total) == 2)
    {
        if (strcmp(label, "MemTotal:") == 0)
        {
            fclose(file);

            return (double)total / 1024.0 / 1024.0;
        }
    }

    fclose(file);

    return -1.0;
}


double get_memory_free(void)
{
    FILE *file;
    char label[64];
    unsigned long long free_memory;

    file = fopen("/proc/meminfo", "r");

    if (file == NULL)
    {
        return -1.0;
    }

    while (fscanf(
        file,
        "%63s %llu kB",
        label,
        &free_memory
    ) == 2)
    {
        if (strcmp(label, "MemAvailable:") == 0)
        {
            fclose(file);

            return (double)free_memory / 1024.0 / 1024.0;
        }
    }

    fclose(file);

    return -1.0;
}


double get_memory_used(void)
{
    double total;
    double free_memory;

    total = get_memory_total();
    free_memory = get_memory_free();

    if (total < 0 || free_memory < 0)
    {
        return -1.0;
    }

    return total - free_memory;
}


/* =========================
   CPU cores
   ========================= */

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


/* =========================
   Uptime
   ========================= */

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


/* =========================
   Load Average
   ========================= */

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


/* =========================
   Disk
   ========================= */

double get_disk_usage(void)
{
    struct statvfs filesystem;

    if (statvfs("/", &filesystem) != 0)
    {
        return -1.0;
    }

    unsigned long long total =
        (unsigned long long)filesystem.f_blocks *
        filesystem.f_frsize;

    unsigned long long available =
        (unsigned long long)filesystem.f_bavail *
        filesystem.f_frsize;

    if (total == 0)
    {
        return -1.0;
    }

    unsigned long long used = total - available;

    return (double)used / total * 100.0;
}


double get_disk_total(void)
{
    struct statvfs filesystem;

    if (statvfs("/", &filesystem) != 0)
    {
        return -1.0;
    }

    unsigned long long total =
        (unsigned long long)filesystem.f_blocks *
        filesystem.f_frsize;

    return (double)total /
           (1024.0 * 1024.0 * 1024.0);
}


double get_disk_used(void)
{
    struct statvfs filesystem;

    if (statvfs("/", &filesystem) != 0)
    {
        return -1.0;
    }

    unsigned long long total =
        (unsigned long long)filesystem.f_blocks *
        filesystem.f_frsize;

    unsigned long long available =
        (unsigned long long)filesystem.f_bavail *
        filesystem.f_frsize;

    unsigned long long used = total - available;

    return (double)used /
           (1024.0 * 1024.0 * 1024.0);
}


double get_disk_available(void)
{
    struct statvfs filesystem;

    if (statvfs("/", &filesystem) != 0)
    {
        return -1.0;
    }

    unsigned long long available =
        (unsigned long long)filesystem.f_bavail *
        filesystem.f_frsize;

    return (double)available /
           (1024.0 * 1024.0 * 1024.0);
}


/* =========================
   Hostname
   ========================= */

int get_hostname(char *buffer, int size)
{
    if (gethostname(buffer, size) != 0)
    {
        return -1;
    }

    buffer[size - 1] = '\0';

    return 0;
}


/* =========================
   Operating System
   ========================= */

int get_os_name(char *buffer, int size)
{
    FILE *file;
    char line[256];

    file = fopen("/etc/os-release", "r");

    if (file == NULL)
    {
        return -1;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "PRETTY_NAME=", 12) == 0)
        {
            char *value = line + 12;

            value[strcspn(value, "\n")] = '\0';

            if (value[0] == '"')
            {
                value++;

                char *quote = strrchr(value, '"');

                if (quote != NULL)
                {
                    *quote = '\0';
                }
            }

            snprintf(buffer, size, "%s", value);

            fclose(file);

            return 0;
        }
    }

    fclose(file);

    return -1;
}


/* =========================
   Kernel
   ========================= */

int get_kernel_version(char *buffer, int size)
{
    FILE *file;
    char line[256];

    file = fopen("/proc/version", "r");

    if (file == NULL)
    {
        return -1;
    }

    if (fgets(line, sizeof(line), file) == NULL)
    {
        fclose(file);

        return -1;
    }

    fclose(file);

    char *start = strstr(line, "Linux version");

    if (start == NULL)
    {
        return -1;
    }

    start += strlen("Linux version");

    while (*start == ' ')
    {
        start++;
    }

    char *end = strchr(start, ' ');

    if (end != NULL)
    {
        *end = '\0';
    }

    snprintf(buffer, size, "%s", start);

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "system_info.h"


void print_system_info(const SystemInfo *info)
{
    printf("================================\n");
    printf("          SYSTEM MONITOR\n");
    printf("================================\n");

    printf("Hostname:        %s\n", info->hostname);
    printf("OS:              %s\n", info->os_name);
    printf("Kernel:          %s\n", info->kernel_version);

    printf("--------------------------------\n");

    printf("CPU Usage:       %6.1f %%\n", info->cpu_usage);

    printf("Memory Usage:    %6.1f %%\n", info->memory_usage);
    printf("Memory Total:    %6.2f GB\n", info->memory_total);
    printf("Memory Used:     %6.2f GB\n", info->memory_used);
    printf("Memory Available:%6.2f GB\n", info->memory_free);

    printf("CPU Cores:       %6d\n", info->cpu_cores);

    printf("Disk Usage:      %6.1f %%\n", info->disk_usage);
    printf("Disk Total:      %6.2f GB\n", info->disk_total);
    printf("Disk Used:       %6.2f GB\n", info->disk_used);
    printf("Disk Available:  %6.2f GB\n", info->disk_available);

    printf("Load Average:    %.2f %.2f %.2f\n",
           info->load1,
           info->load5,
           info->load15);

    printf("Uptime:          %s\n", info->uptime);

    printf("================================\n");
}


int main(int argc, char *argv[])
{
    /*
     * Обробка командного рядка
     */

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


    /*
     * Структура з інформацією про систему
     */

    SystemInfo info;


    /*
     * Статична інформація
     */

    if (get_hostname(info.hostname, sizeof(info.hostname)) != 0)
    {
        printf("Error: cannot read hostname\n");
        return 1;
    }

    if (get_os_name(info.os_name, sizeof(info.os_name)) != 0)
    {
        printf("Error: cannot read operating system information\n");
        return 1;
    }

    if (get_kernel_version(info.kernel_version,
                           sizeof(info.kernel_version)) != 0)
    {
        printf("Error: cannot read kernel information\n");
        return 1;
    }

    info.cpu_cores = get_cpu_cores();

    if (info.cpu_cores < 0)
    {
        printf("Error: cannot read CPU core information\n");
        return 1;
    }


    /*
     * Real-time monitor
     */

    while (1)
    {
        /*
         * Очищення термінала
         *
         * \033[2J  - очистити екран
         * \033[H   - перемістити курсор у верхній лівий кут
         */

        printf("\033[2J\033[H");


        /*
         * Отримуємо актуальні дані
         */

        info.cpu_usage = get_cpu_usage();

        info.memory_usage = get_memory_usage();
        info.memory_total = get_memory_total();
        info.memory_used = get_memory_used();
        info.memory_free = get_memory_free();

        info.disk_usage = get_disk_usage();
        info.disk_total = get_disk_total();
        info.disk_used = get_disk_used();
        info.disk_available = get_disk_available();

        get_load_average(
            &info.load1,
            &info.load5,
            &info.load15
        );

        get_uptime(
            info.uptime,
            sizeof(info.uptime)
        );


        /*
         * Перевірка помилок
         */

        if (info.cpu_usage < 0 ||
            info.memory_usage < 0 ||
            info.memory_total < 0 ||
            info.memory_used < 0 ||
            info.memory_free < 0 ||
            info.disk_usage < 0 ||
            info.disk_total < 0 ||
            info.disk_used < 0 ||
            info.disk_available < 0 ||
            info.load1 < 0)
        {
            printf("Error: cannot read system information\n");
            return 1;
        }


        /*
         * Виведення
         */

        print_system_info(&info);

        printf("\n");
        printf("Refreshing every 1 second...\n");
        printf("Press Ctrl+C to exit.\n");


        /*
         * Очікуємо 1 секунду
         */

        sleep(1);
    }


    return 0;
}

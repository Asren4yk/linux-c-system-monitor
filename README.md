# Linux System Monitor

A lightweight command-line system monitor written in C for Linux.

The program reads system information directly from the Linux system and
displays CPU, memory, disk, load average, uptime and other system statistics
in a simple terminal interface.

## Features

- CPU usage
- Memory usage
- Total, used and available memory
- CPU core count
- Disk usage
- Total, used and available disk space
- Load average (1, 5 and 15 minutes)
- System uptime
- Hostname
- Operating system information
- Kernel version
- Real-time updates every second
- Command-line options
- Basic error handling

## Requirements

- Linux
- GCC
- GNU Make
- C11-compatible compiler

## Build

Clone the repository and build the project:

```bash
git clone https://github.com/Asren4yk/linux-c-system-monitor.git
cd linux-c-system-monitor
make

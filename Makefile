CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = sys-monitor

SRC = main.c system_info.c
OBJ = main.o system_info.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

main.o: main.c system_info.h
	$(CC) $(CFLAGS) -c main.c

system_info.o: system_info.c system_info.h
	$(CC) $(CFLAGS) -c system_info.c

clean:
	rm -f $(OBJ) $(TARGET)

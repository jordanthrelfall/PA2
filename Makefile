CC = gcc
CFLAGS = -Wall -Wextra -pthread

SRCS = chash.c hashdb.c rwlocks.c
OBJS = $(SRCS:.c=.o)
HEADERS = common_threads.h common.h hashdb.h rwlocks.h

TARGET = chash

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
CFLAGS = -g -Wall
CC = gcc
TARGET = minesweeper

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(OUTPUT_OPTION) $(CFLAGS) $^

clean:
	$(RM) $(OBJS) $(TARGET)

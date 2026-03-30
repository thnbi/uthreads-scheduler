CC = gcc
CFLAGS = -Wall -Wextra -I include
SRCDIR = src
BUILDDIR = build

SRCS = thread.c scheduler.c dispatcher.c signal_handler.c main.c
OBJS = $(patsubst %.c,$(BUILDDIR)/%.o,$(SRCS))

all: $(BUILDDIR) scheduler signal_sender

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

scheduler: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

signal_sender: $(BUILDDIR)/signal_sender.o
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) scheduler signal_sender

.PHONY: all clean

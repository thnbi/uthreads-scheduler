CC = gcc
CFLAGS = -Wall -Wextra -I include
SRCDIR = src
BUILDDIR = build

SRCS = thread.c scheduler.c dispatcher.c signal_handler.c semaphore.c concurrency.c main.c
OBJS = $(patsubst %.c,$(BUILDDIR)/%.o,$(SRCS))

all: $(BUILDDIR) scheduler

scheduler: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) scheduler

.PHONY: all clean

CC = gcc
CFLAGS = -Wall -Wextra

ALGS = fifo lru optimal lfu mfu clock

all: $(ALGS)

$(ALGS): %: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(ALGS)

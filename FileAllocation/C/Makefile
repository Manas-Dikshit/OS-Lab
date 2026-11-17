CC = gcc
CFLAGS = -Wall -Wextra

ALGS = sequential linked indexed

all: $(ALGS)

$(ALGS): %: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(ALGS) $(addsuffix .exe,$(ALGS))

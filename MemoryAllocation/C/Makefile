CC = gcc
CFLAGS = -Wall -Wextra

ALGS = first_fit best_fit worst_fit

all: $(ALGS)

$(ALGS): %: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(ALGS)

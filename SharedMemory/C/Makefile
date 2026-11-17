CC = gcc
CFLAGS = -Wall -Wextra
LIBS =

ifeq ($(shell uname -s),Linux)
LIBS = -lrt
endif

PROG = shared_memory

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(PROG) $(PROG).exe

CC = gcc

ifneq ($(filter Linux Darwin,$(shell uname -s)),)
	LDLIBS = -pthread
endif

producer_consumer: producer_consumer.c
	$(CC) -Wall -Wextra $< -o $@ $(LDLIBS)

clean:
	rm -f producer_consumer
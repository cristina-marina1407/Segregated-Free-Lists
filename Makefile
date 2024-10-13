CC=gcc
CFLAGS= -Wall -Wextra -std=c99

TARGETS = sfl

build: $(TARGETS)

sfl: sfl.c
	$(CC) $(CFLAGS) sfl.c -lm -o sfl

run_sfl: build
	./$(TARGETS)

clean:
	rm -f $(TARGETS)

CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c11
EXE=wc

.PHONY: all
all: clean build

.PHONY: build
build:
	$(CC) $(CFLAGS) src/word_frequency.c src/hashtable.c src/config.c  src/main.c -o $(EXE)
#
.PHONY: clean
clean:
	rm -f $(EXE)

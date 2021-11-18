CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wpedantic -std=c11 -g

EXE=wf

.PHONY: all
all: clean build

.PHONY: build
build:
	$(CC) $(CFLAGS) src/word_frequency.c src/hashtable.c src/config.c  src/main.c -o $(EXE)
#
.PHONY: clean
clean:
	rm -f $(EXE)

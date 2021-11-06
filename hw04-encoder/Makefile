CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c11
EXE=encoder

.PHONY: all
all: clean build

.PHONY: build
build:
	$(CC) $(CFLAGS) src/config.c src/main.c -o $(EXE)

.PHONY: clean
clean:
	rm -f $(EXE)

CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c11
EXE=zipjpeg

.PHONY: all
all: clean build

.PHONY: build
build:
	$(CC) $(CFLAGS) main.c -o $(EXE)

.PHONY: clean
clean:
	rm -f $(EXE)

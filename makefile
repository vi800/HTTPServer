# vim: noexpandtab

SOURCES = $(wildcard src/*.c)

P = main
CFLAGS = -g -Wall -Ihdr
CC = c99

$(P): $(SOURCES)
	$(CC) $(CFLAGS) -o $(P) $(SOURCES)


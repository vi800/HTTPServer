# vim: noexpandtab

SOURCES = $(wildcard src/*.c)

P = main
CFLAGS = -g -Wall -Ihdr
CC = gcc

ifeq ($(OS), Windows_NT)
	LIBS = -lws2_32
else 
	LIBS = -pthread
endif

$(P): $(SOURCES)
	$(CC) $(CFLAGS) -o $(P) $(SOURCES) $(LIBS)

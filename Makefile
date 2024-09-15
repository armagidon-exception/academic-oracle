CFLAGS=-std=c99 -Wall -g -Isrc/
CC=gcc
APPNAME=bot
LIBS= -ltelebot -lsqlite3
CFLAGS += $(LIBS)
# CFLAGS += -fsanitize=address

obj/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
	$(CC) $(CFLAGS) $^ -o $@

CC=gcc
CFLAGS= -g -Wall -Wextra -pedantic

all : brightness

brightness : brightness.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY : all clean

clean :
	rm brightness

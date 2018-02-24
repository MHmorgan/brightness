CC=gcc
CFLAGS= -Wall -Wextra -pedantic

all : brightness

brightness : brightness.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY : all clean install

install : all
	sudo chown root brightness
	sudo chmod 4755 brightness
	sudo mv brightness ~/bin

clean :
	rm brightness

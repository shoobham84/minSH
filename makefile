CC=gcc
CFLAGS=-Wall -Wextra -g 

SHELL_ROOT=$(CURDIR)
COREUTILS_PATH=$(SHELL_ROOT)/coreutils

all: coreutils shell

shell: src/shell.c src/parser.c
	$(CC) $(CFLAGS) -Isrc src/shell.c src/parser.c -o shell -D COREUTILS_PATH='"$(COREUTILS_DIR)"'

clean:
	rm -f shell

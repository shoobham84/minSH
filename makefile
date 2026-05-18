CC=gcc
CFLAGS= -std=c2x -D_POSIX_C_SOURCE=200809L  -Wall -Wextra -g 

SHELL_ROOT=$(CURDIR)
COREUTILS_PATH=$(SHELL_ROOT)/coreutils

all: coreutils minsh 

minsh: src/shell.c src/parser.c
	$(CC) $(CFLAGS) -Isrc src/shell.c src/parser.c -o minsh -D COREUTILS_PATH='"$(COREUTILS_PATH)"'

clean:
	rm -f minsh

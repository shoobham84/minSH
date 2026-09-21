#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

void usage_error(const char* format, ...) {
	va_list arg_list;

	fprintf(stderr, "Usage: ");
	va_start(arg_list, format);
	vfprintf(stderr, format, arg_list);
	va_end(arg_list);

	fflush(stderr);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	int open_flags = O_CREAT | O_WRONLY | O_TRUNC ;
	mode_t file_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ;
	char buf[BUF_SIZE];

	if (argc != 3 || strcmp(argv[1], "-h") == 0) {
		usage_error("%s <old-file> <new-file>\n", argv[0]);
		exit(-1);
	}

	int input_fd = open(argv[1], O_RDONLY);
	if (input_fd == -1) {
		fprintf(stderr, "couldn't open file %s", argv[1]);
		exit(EXIT_FAILURE);
	}

	int output_fd = open(argv[2], open_flags, file_perms);
	if (output_fd == -1) {
		fprintf(stderr, "couldn't open file %s", argv[2]);
		exit(EXIT_FAILURE);
	}

	ssize_t num_read;
	while((num_read = read(input_fd, buf, BUF_SIZE)) > 0) {
		if (write(output_fd, buf, num_read) != num_read) {
			perror("couldn't write whole buffer");
			exit(EXIT_FAILURE);
		}
	}

	if (num_read == -1) {
		perror("couldn't read");
		exit(EXIT_FAILURE);
	}

	if (close(input_fd) == -1) {
		perror("couldn't close input file");
		exit(EXIT_FAILURE);
	}

	if (close(output_fd) == -1) {
		perror("couldn't close output file");
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}

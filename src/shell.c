#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "command.h"
#include "parser.h"

#ifndef COREUTILS_PATH
    #define COREUTILS_PATH "/dev/null"
#endif

int interactive = 0;
int shell_init = 1;

int main() {
    interactive = isatty(STDIN_FILENO) && isatty(STDERR_FILENO);

    /* int shell_init = 0;   upon sighandling */
    while(shell_init) {
        if (interactive) {
            printf("min> ");
            fflush(stdout); 
        }
        
        char *buff = NULL;
        size_t len = 0;
        
        /* basic getline instead of read syscall */
        ssize_t bytes_read = getline(&buff, &len, stdin);
        if (bytes_read == -1) {
            if (interactive) {
                printf("\n");
            }
            free(buff);
            break;
        }
        COMMAND *command = parse_line(buff, bytes_read);

        if (command != NULL) {
            free_command(command);
        }
        free(buff); 
    }
    
    return EXIT_SUCCESS;
}

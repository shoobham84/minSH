#define _POSIX_C_SOURCE 200809L   /* for getline read loop */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "command.h"

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
        if (getline(&buff, &len, stdin) == -1) {
            if (interactive) {
                printf("\n");
            }
            free(buff);
            break;
        }
        // printf("%s", buff);
        free(buff); 
    }
    
    return 0;
}

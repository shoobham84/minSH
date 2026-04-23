#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "command.h"

int interactive = 0;

int main() {
    interactive = isatty(STDIN_FILENO);

    /* int shell_init = 0;   upon sighandling */
    while(1) {
        if (interactive) {
            printf("min> ");
            fflush(stdout); 
        }
        
        char *buff = NULL;
        size_t len = 0;
        
        // basic getline instead of read syscall
        if (getline(&buff, &len, stdin) == -1) {
            if (interactive) {
                printf("\n");
            }
            free(buff);
            break;
        }
        
        free(buff); 
    }
    
    return 0;
}

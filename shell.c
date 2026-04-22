#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int interactive = isatty(STDIN_FILENO);

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

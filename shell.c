#include <stdio.h>

int main() {
    while(1) {
        printf("min> ");
        char *buff = NULL;
        size_t len = 0;
        if (getline(&buff, &len, stdin) == -1) {
            printf("\n");
            break;
        }
    }
}

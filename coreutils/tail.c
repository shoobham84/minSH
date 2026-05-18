/* tail -  prints last n lines of its input. n is set to 10 by default
 * tail -n for altering n */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_LINES 10

int main(int argc, char **argv) {
    int n = DEFAULT_LINES;

    while(--argc > 0 && (*++argv)[0] == '-') {
        if (*(argv + 1) != NULL) {
            switch ((*argv)[1]) {
                case 'n':
                    n = atoi(*++argv);
                    --argc;
                    break;
                default:
                    fprintf(stderr, "error: illegal flag\n");
                    return EXIT_FAILURE;
            }
        }
        else if (strlen(*argv) > 2)
            n = atoi((*argv) + 2);
    }

    if (n <= 0)
        n = DEFAULT_LINES;

    char **line_ptrs = calloc(n, sizeof(char *));
    size_t *line_sizes = calloc(n, sizeof(size_t));

    if (line_ptrs == NULL || line_sizes == NULL) {
        fprintf(stderr, "error: out of memory initializing arrays\n");
        return EXIT_FAILURE;
    }

    int current_idx = 0; 
    int total_lines = 0; /* total lines of input */

    while(getline(&line_ptrs[current_idx], &line_sizes[current_idx], stdin) != -1) {
        current_idx = (current_idx + 1) % n; /* uses a circular buffer, instead of reading all lines, it only reads the last n lines */
        total_lines++;
    }

    int print_count = (total_lines < n) ? total_lines : n;
    int start_idx = (total_lines < n) ? 0 : current_idx;

    for (int i = 0; i < print_count; ++i) {
        int idx = (start_idx + i) % n; 
        if (line_ptrs[idx] != NULL)
            printf("%s", line_ptrs[idx]);
    }

    for (int i = 0; i < n; ++i)
        free(line_ptrs[i]);

    free(line_ptrs);
    free(line_sizes);

    return EXIT_SUCCESS;
}

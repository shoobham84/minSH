#ifndef PARSE_H
#define PARSE_H

#include "command.h"
#include <stddef.h>

COMMAND *parse_line(const char *line, size_t len); /* take raw string and return AST root upon parsing */

void free_command(COMMAND *command); /* recursively free struct and words/redirects */

#endif /* PARSE_H */

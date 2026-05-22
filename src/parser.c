/* parser.c - handles all parsing operations of the shell */
#include "parser.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static WORD_DESC *tokenizer(const char *line, size_t len, size_t *posn);
static WORD_LIST *token_list(char *line, size_t len);
static COMMAND *parse_tokens(WORD_LIST **rest);

static WORD_DESC *tokenizer(const char *line, size_t len, size_t *posn) 
{
    while(*posn < len && isspace((unsigned char)line[*posn]))
        (*posn)++;

    WORD_DESC *word = calloc(1, sizeof *word); 
    if (word == NULL)
        return NULL;

    size_t start = *posn;
    char c;

    /* checks for quoted strings in the command */
    if ((c = line[start]) == '"' || c == '\'') {
        word->flags = W_QUOTED;
        char quote = c;
        (*posn)++;

        size_t qstart = *posn;
        while(*posn < len && line[*posn] != quote)
            (*posn)++;

        word->word = strndup(line+qstart, *posn-qstart);

        return word;
    }

    /* checks for variables in the command; variables in minsh start with a '$' */
    if (c == '$') {
        word->flags = W_VARIABLE;
        (*posn)++;

        while(*posn < len && !isspace((unsigned char)line[*posn])) 
            (*posn)++;

        word->word = strndup(line+start, *posn-start);
        return word;
    }
}

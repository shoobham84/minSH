/* parser.c - handles all parsing operations of the shell */
#include "parser.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static WORD_DESC *tokenizer(const char *line, size_t len, size_t *posn);
static WORD_LIST *token_list(char *line, size_t len);
static COMMAND *parse_tokens(WORD_LIST **rest);

/* auxilliary functions */
static int is_char_quote(char c) {
    return c == '"' || c == '\'';
}

static int is_char_variable(char c) {
    return c == '$';
}

static int is_char_operator(char c) {
    return c == '|' || c == '&' || c == ';' || c == '<' || c == '>';
}

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
    if (is_char_quote((c = line[start]))) {
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
    if (is_char_variable(c)) {
        word->flags = W_VARIABLE;
        (*posn)++;

        while(*posn < len && !isspace((unsigned char)line[*posn])) 
            (*posn)++;

        word->word = strndup(line+start, *posn-start);
        return word;
    }
}

/* parser.c - LL(1) parser handling all parsing operations of the shell */
#include "parser.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static WORD_DESC *tokenizer(const char *line, size_t len, size_t *posn);
static WORD_LIST *token_list(const char *line, size_t len);
static COMMAND *parse_simple(WORD_LIST **token_stream);

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

static CONNECTOR connector_for(WORD_LIST token) {
    char *connector = token.word->word;

    if (strcmp(connector, "|") == 0) 
        return C_PIPE;
    
    else if (strcmp(connector, ";") == 0) 
        return C_SEMI;
    
    else if (strcmp(connector, "&&") == 0) 
        return C_AND;
    
    else if (strcmp(connector, "||") == 0) 
        return C_OR;
    
    return -1;
}

static int scan_operator(const char *line, size_t len, size_t posn, size_t *op_len) {
    static const struct { 
        const char *str;
        size_t slen;
    } ops[] = {
        { ">>", 2 },
        { "&&", 2 },
        { "||", 2 },
        { "|",  1 },
        { "&",  1 },
        { ";",  1 },
        { "<",  1 },
        { ">",  1 },
    };

    size_t remaining = len - posn;

    for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); ++i) {
        if (ops[i].slen <= remaining && strncmp(line + posn, ops[i].str, ops[i].slen) == 0) {
            *op_len = ops[i].slen;
            return 1;
        }
    }
    return 0;
}

/* main parser functions */

/* tokenizer
 * scans from line[*pos] (line is the word being parsed) and returns the next WORD_DESC token.
 * returns a NULL when line fully iterated upon 
 * */
static WORD_DESC *tokenizer(const char *line, size_t len, size_t *posn) 
{
    while(*posn < len && isspace((unsigned char)line[*posn]))
        (*posn)++;

    if (*posn >= len)
        return NULL;

    WORD_DESC *word = calloc(1, sizeof *word); 
    if (word == NULL)
        return NULL;

    size_t start = *posn;
    char c; 

    if (is_char_quote((c = line[start]))) {
        word->flags = W_QUOTED;
        char quote = c;
        (*posn)++;

        size_t qstart = *posn;
        while(*posn < len && line[*posn] != quote)
            (*posn)++;

        word->word = strndup(line+qstart, *posn-qstart);

        if (*posn < len)
            (*posn)++;

        return word;
    }

    size_t op_len;
    if (is_char_operator(c) && scan_operator(line, len, start, &op_len)) {
        word->flags = W_OPERATOR;
        *posn += op_len;
        word->word = strndup(line + start, op_len);
        return word;
    }

    if (is_char_variable(c)) {
        word->flags = W_VARIABLE;
        (*posn)++;

        while(*posn < len && !isspace((unsigned char)line[*posn])) 
            (*posn)++;

        word->word = strndup(line+start, *posn-start);
        return word;
    }

    while (*posn < len && !isspace((unsigned char)line[*posn]) && !is_char_operator(line[*posn]) && !is_char_quote(line[*posn]))
        (*posn)++;

    word->word = strndup(line+start, *posn-start);
    word->flags = W_NORMAL;
    return word;
}

/* token_list
 * calls tokenizer() repeatedly and links every WORD_DESC into a linked list of WORD_LIST while preserving the input order
 */
static WORD_LIST *token_list(const char *line, size_t len)
{
    WORD_LIST dummy_head;          
    dummy_head.word = NULL;
    dummy_head.next = NULL;

    WORD_LIST *tail = &dummy_head;  

    size_t posn = 0;

    while(posn < len) {
        WORD_DESC *wrd = tokenizer(line, len, &posn);
        if (wrd == NULL)
            break;

        WORD_LIST *token_node = calloc(1, sizeof *token_node);
        if (token_node == NULL) {
            free(wrd->word);
            free(wrd);
            break;
        }

        token_node->word = wrd;         
        token_node->next = NULL;
        tail->next = token_node;
        tail=token_node;
    }
    return dummy_head.next;   
}

static COMMAND *parse_simple(WORD_LIST **token_stream) 
{
    if (token_stream == NULL || *token_stream == NULL)
        return NULL;

    COMMAND *cmd = calloc(1, sizeof *cmd);
    if (cmd == NULL) 
        return NULL;

    cmd->type = C_SIMPLE;
    cmd->flags = 0;
    
    WORD_LIST *wl_tail = NULL;

    while(*token_stream) {
        WORD_DESC *word = (*token_stream)->word;

        if (word->flags & W_OPERATOR)
            break;

        WORD_LIST *curr = *token_stream;
        *token_stream = curr->next;

        curr->next = NULL;
        if (wl_tail == NULL)
            cmd->value.simple.words = curr;
        else
            wl_tail->next = curr;
        wl_tail = curr;

    }

    return cmd;
}

/* parse_connection
 * builds the connection tree (pipes, &&, ||, ;)
 */
static COMMAND *parse_connection(WORD_LIST **token_stream) {
    COMMAND *leftside = parse_simple(token_stream);
    if (leftside == NULL)
        return NULL;

    while(*token_stream) {
        CONNECTOR connector_type = connector_for(**token_stream);
        if (connector_type == -1)
            break;

        WORD_DESC *connector = (*token_stream)->word;

        WORD_LIST *node = *token_stream;
        *token_stream = node->next;
        
        free(connector->word);
        free(connector);      
        free(node);           

        COMMAND *rightside = parse_simple(token_stream);
        COMMAND *final_cmd = calloc(1, sizeof *final_cmd);
        if (final_cmd == NULL)
            return NULL;

        final_cmd->type = C_CONNECTION;
        final_cmd->flags = 0;
        final_cmd->value.connection.first = leftside;
        final_cmd->value.connection.second = rightside;
        final_cmd->value.connection.connector = connector_type;

        leftside = final_cmd;
    }
    return leftside;
}

/*command.h -- structures used internally to represent commands*/

#if !defined(_COMMAND_H_)
#define _COMMAND_H_

/* values for flags for WORD_DESC */
#define W_NORMAL 0
#define W_QUOTED (1 << 0) /* Word contains quotes, do not split */
#define W_VARIABLE (1 << 1) /* Word starts with $, req. env lookup */
#define W_OPERATOR (1 << 2) /* Word is a shell operator {|, <, >, &, ;, >>}*/

/* structure representing a word */
typedef struct word_desc {
    char *word;
    int flags;
} WORD_DESC;


/* linked list of words */
typedef struct word_list {
    struct word_list *next;
    WORD_DESC *word;
} WORD_LIST;

typedef enum {
    R_INPUT_DIRN,  /* < */
    R_OUTPUT_DIRN, /* > */
    R_APPENDTO_DIRN, /* >> */
} REDIRECT_INSTRUCTION;

typedef struct redirect {
    struct redirect *next;
    REDIRECT_INSTRUCTION instruction;
    WORD_DESC *redirectee;

} REDIRECT;


typedef struct command {
    int flags;
    int line;

} COMMAND;

#endif 

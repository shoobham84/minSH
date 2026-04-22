/* data structures used by shell */
#include <sys/types.h>

/* reference: official bash documentation */
struct user_info {
    uid_t uid;
    char *user_name;
    char *shell;
    char *home_dir;
};

extern struct user_info current_user;

extern int interactive;
extern int shell_init;



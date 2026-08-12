#include <stdio.h>
#include <stdlib.h>

/* Shell commands for the current platform. */
static const char *const unix_cmds[] = {
    "ls",
    "cp directory_setup.c newdir/SystemCopy.c",
    "ls newdir",
    "grep main directory_setup.c"
};

static const char *const win_cmds[] = {
    "dir",
    "copy directory_setup.c newdir\\SystemCopy.c",
    "dir newdir",
    "findstr main directory_setup.c"
};

static const char *const labels[] = {
    "ls", "cp", "ls", "grep"
};

int main(void)
{
#ifdef _WIN32
    const char *const *cmds = win_cmds;
#else
    const char *const *cmds = unix_cmds;
#endif
    const int count = (int)(sizeof(labels) / sizeof(labels[0]));

    /* Walk the array with a pointer (const char * const *p). */
    const char *const *p = cmds;
    for (int i = 0; i < count; i++, p++) {
        printf("------- %s -------\n", labels[i]);
        fflush(stdout);

        int code = system(*p);
        printf("Exit code: %d\n", code);
        printf("\n");
    }

    return 0;
}
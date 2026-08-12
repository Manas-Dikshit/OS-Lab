#include "osport.h"

int main(int argc, char **argv)
{
    printf("Child process PID: %ld\n", (long)os_getpid());

    /* argv is a pointer to an array of char* pointers (char**). */
    char **argp = argv;
    if (argc > 0 && argp[1] != NULL) {
        printf("Child received: %s\n", argp[1]);
    }

    return 0;
}
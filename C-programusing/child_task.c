#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    printf("Child process PID: %d\n", getpid());

    /* argv is a pointer to an array of char* pointers (char**). */
    char **argp = argv;
    if (argc > 0 && argp[1] != NULL) {
        printf("Child received: %s\n", argp[1]);
    }

    return 0;
}
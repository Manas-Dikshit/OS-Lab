#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char **argv)
{
    /* Select the path: default "." unless an argument is given (char*). */
    const char *path = (argc > 1) ? argv[1] : ".";

    /* opendir() returns a pointer to a DIR stream. */
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        fprintf(stderr, "opendir/readdir/closedir: %s\n", path);
        return 1;
    }

    printf("Contents of directory '%s':\n", path);

    /* readdir() returns a pointer to a struct dirent entry. */
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("  %s\n", entry->d_name);
    }

    closedir(dir);
    printf("Directory listing complete.\n");
    return 0;
}
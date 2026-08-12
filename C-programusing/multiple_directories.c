#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void list_directory(const char *path)
{
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        fprintf(stderr, "opendir/readdir/closedir for '%s': %s\n", path, strerror(errno));
        return;
    }

    printf("Contents of directory '%s':\n", path);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("  %s\n", entry->d_name);
    }

    closedir(dir);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <dir1> <dir2> ...\n", argv[0]);
        return 1;
    }

    /* Iterate over each argument. argv is a char** (pointer to pointers). */
    char **argp = argv + 1;
    int count = argc - 1;
    for (int i = 0; i < count; i++) {
        list_directory(argp[i]);
    }

    printf("Directory listing complete.\n");
    return 0;
}
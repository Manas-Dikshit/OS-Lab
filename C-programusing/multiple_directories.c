#include "osport.h"

static void list_directory(const char *path)
{
    OS_DIR *dir = os_opendir(path);
    if (!dir) {
        printf("opendir/readdir/closedir for '%s': cannot open\n", path);
        return;
    }

    printf("Contents of directory '%s':\n", path);

    const char *entry;
    while ((entry = os_readdir(dir)) != NULL) {
        printf("  %s\n", entry);
    }

    os_closedir(dir);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: %s <dir1> <dir2> ...\n", argv[0]);
        return 1;
    }

    /* Iterate over the arguments. argv is a char** (pointer to pointers). */
    char **argp = argv + 1;
    int count = argc - 1;
    for (int i = 0; i < count; i++) {
        list_directory(argp[i]);
    }

    printf("Directory listing complete.\n");
    return 0;
}
#include "osport.h"

int main(int argc, char **argv)
{
    /* Path selects "." unless an argument is given. argv[1] is a char*. */
    const char *path = (argc > 1) ? argv[1] : ".";

    OS_DIR *dir = os_opendir(path);
    if (!dir) {
        printf("opendir/readdir/closedir: cannot open '%s'\n", path);
        printf("Directory listing complete.\n");
        return 1;
    }

    printf("Contents of directory '%s':\n", path);

    const char *entry;
    while ((entry = os_readdir(dir)) != NULL) {
        printf("  %s\n", entry);
    }

    os_closedir(dir);
    printf("Directory listing complete.\n");
    return 0;
}
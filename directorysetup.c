#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <dirent.h>

int main(void) {
    const char *path = "newdir";

    if (_mkdir(path) == 0) {
        printf("Directory '%s' created successfully.\n", path);
    } else {
        perror("mkdir");
    }

    printf("\nContents of current directory:\n");
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    struct dirent *entry;
    int found = 0;
    while ((entry = readdir(dir)) != NULL) {
        printf("  %s\n", entry->d_name);
        found = 1;
    }
    closedir(dir);

    if (!found) {
        printf("  (no files found)\n");
    }

    printf("\nDirectory listing complete.\n");
    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <string.h>
#include "osport.h"

/*
 * Executes the same "utility" tasks as the Java SyscallCommands program
 * (list a directory, copy a file, search text) but WITHOUT ever spawning a
 * shell. No system() is used, so no command string is ever interpreted by a
 * shell. All files and directories touched are local to the current folder.
 */

static void list_dir(const char *path)
{
    OS_DIR *dir = os_opendir(path);
    if (!dir) {
        printf("cannot open '%s'\n", path);
        return;
    }
    const char *entry;
    while ((entry = os_readdir(dir)) != NULL) {
        printf("  %s\n", entry);
    }
    os_closedir(dir);
}

static int copy_file(const char *src, const char *dst)
{
#ifdef _WIN32
    return CopyFileA(src, dst, FALSE) ? 0 : -1;
#else
    /* cp with no shell: argv is a char** (pointer to char* pointers). */
    char *const av[] = { "cp", (char *)src, (char *)dst, NULL };
    int code = 0;
    return os_exec_argv(av, &code) == 0 ? 0 : -1;
#endif
}

static int grep_file(const char *pattern, const char *path)
{
#ifdef _WIN32
    /* In-process search: no external program, no shell, fully local. */
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[4096];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, pattern)) {
            found = 1;
            printf("%s", line);
        }
    }
    fclose(f);
    return found ? 0 : 1;
#else
    /* grep with no shell. */
    char *const av[] = { "grep", (char *)pattern, (char *)path, NULL };
    int code = 0;
    os_exec_argv(av, &code);
    return code;
#endif
}

int main(void)
{
    const char *src = "directory_setup.c";
    const char *dst = "newdir/SystemCopy.c";

    printf("------- ls -------\n");
    list_dir(".");
    printf("Exit code: 0\n\n");

    printf("------- cp -------\n");
    /* Ensure the target folder exists (local to the current folder). */
    os_mkdir("newdir");
    int rc = copy_file(src, dst);
    printf("Exit code: %d\n\n", rc);

    printf("------- ls -------\n");
    list_dir("newdir");
    printf("Exit code: 0\n\n");

    printf("------- grep -------\n");
    rc = grep_file("main", src);
    printf("Exit code: %d\n\n", rc);

    return 0;
}
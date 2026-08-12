#include "osport.h"

int main(void)
{
    printf("Parent process PID: %ld\n", (long)os_getpid());

    /* Build the path of the child executable for the current platform. */
    char child_name[4096];
    snprintf(child_name, sizeof(child_name),
             ".%s%s%s", OS_PATH_SEP, "child_task", OS_EXE_SUFFIX);

    os_pid_t child_pid = 0;
    int code = 0;
    int rc = os_run_child(child_name, "Hello from child process", &child_pid, &code);

    if (rc == 0) {
        printf("Child process PID: %ld\n", (long)child_pid);
        printf("Child exited with code: %d\n", code);
    } else {
        printf("Failed to spawn child process\n");
    }

    return 0;
}
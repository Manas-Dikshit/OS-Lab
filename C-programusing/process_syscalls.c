#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    printf("Parent process PID: %d\n", getpid());

    /* Fork a child process. */
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /* Child: replace its image with the child_task program (execve). */
        char *path = "./child_task";
        char *const child_argv[] = { path, "Hello from child process", NULL };
        char *const envp[] = { NULL };
        execve(path, child_argv, envp);

        /* Only reached if exec fails. */
        perror("execve");
        _exit(127);
    }

    /* Parent: wait for the child to finish. */
    int status = 0;
    waitpid(pid, &status, 0);

    int code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    printf("Child exited with code: %d\n", code);

    return 0;
}
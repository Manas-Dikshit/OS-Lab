#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void run_command(const char *name)
{
    printf("------- %s -------\n", name);

    /* Fork a child process. */
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        /* Child: replace its image with bash running the given shell commands. */
        char *const child_argv[] = {
            "/bin/bash", "-c",
            /* Each invocation mirrors one command in the Java original. */
            "ls",
            NULL
        };

        /* Switch which command to run based on the pointer name. */
        if (name[0] == 'c') {
            child_argv[2] = "cp multiple_directories.c newdir/SyscallCommandsCopy.c";
        } else if (name[0] == 'g') {
            child_argv[2] = "grep main multiple_directories.c";
        }

        execve("/bin/bash", child_argv, NULL);
        perror("execve");
        _exit(127);
    }

    /* Parent: wait for the child to finish. */
    int status = 0;
    waitpid(pid, &status, 0);
    int code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    printf("Exit code: %d\n", code);
    printf("\n");
}

int main(void)
{
    run_command("ls");
    run_command("cp");
    run_command("ls");
    run_command("grep");
    return 0;
}
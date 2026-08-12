#ifndef OSPORT_H
#define OSPORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
 * Cross-platform operating-system helpers.
 *
 * On Windows (when _WIN32 is defined) the POSIX headers unistd.h,
 * sys/wait.h and dirent.h do not exist, so this header provides its own
 * implementations using the Windows APIs. On Linux/macOS it simply uses the
 * native POSIX calls. Every program in this folder only needs this header,
 * so it can be compiled on any system without include errors.
 *
 * SECURITY NOTES:
 *   - Every helper here only touches the current working directory and the
 *     calling process. Nothing reads or writes other devices, nothing opens
 *     a network connection, and no privilege is ever requested or elevated.
 *   - Child processes are started with execv / CreateProcess ONLY, never
 *     through a shell, so no command string is ever interpreted by a shell
 *     (no shell-injection surface). All argument buffers are bounded with
 *     snprintf / strncpy.
 */

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #define OS_PATH_SEP "\\"
    #define OS_EXE_SUFFIX ".exe"
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <sys/stat.h>
    #include <dirent.h>
    #define OS_PATH_SEP "/"
    #define OS_EXE_SUFFIX ""
#endif

#ifdef _WIN32
typedef DWORD os_pid_t;
#else
typedef pid_t os_pid_t;
#endif

/* ---------------- current process ID ---------------- */

static inline os_pid_t os_getpid(void)
{
#ifdef _WIN32
    return GetCurrentProcessId();
#else
    return (os_pid_t)getpid();
#endif
}

/* ---------------- create a single directory (local) ---------------- */

static inline int os_mkdir(const char *path)
{
#ifdef _WIN32
    if (_mkdir(path) == 0) return 0;
    return (errno == EEXIST) ? 0 : -1;
#else
    if (mkdir(path, 0700) == 0) return 0;
    return (errno == EEXIST) ? 0 : -1;
#endif
}

/* ---------------- directory iteration (opendir/readdir/closedir) ---------------- */

#ifdef _WIN32
typedef struct OS_DIR {
    HANDLE hFind;
    WIN32_FIND_DATAA ffd;
    char name[4096];
    int first;
} OS_DIR;
#else
typedef DIR OS_DIR;
#endif

static inline OS_DIR *os_opendir(const char *path)
{
#ifdef _WIN32
    OS_DIR *d = (OS_DIR *)malloc(sizeof(OS_DIR));
    if (!d) return NULL;
    {
        char pattern[4096];
        snprintf(pattern, sizeof(pattern), "%s%s*", path, OS_PATH_SEP);
        d->hFind = FindFirstFileA(pattern, &d->ffd);
        if (d->hFind == INVALID_HANDLE_VALUE) {
            free(d);
            return NULL;
        }
        d->first = 1;
        d->name[0] = '\0';
    }
    return d;
#else
    return opendir(path);
#endif
}

static inline const char *os_readdir(OS_DIR *d)
{
#ifdef _WIN32
    if (!d) return NULL;
    for (;;) {
        BOOL ok = d->first ? TRUE : FindNextFileA(d->hFind, &d->ffd);
        d->first = 0;
        if (!ok) return NULL;
        if (strcmp(d->ffd.cFileName, ".") == 0 || strcmp(d->ffd.cFileName, "..") == 0)
            continue;
        strncpy(d->name, d->ffd.cFileName, sizeof(d->name) - 1);
        d->name[sizeof(d->name) - 1] = '\0';
        return d->name;
    }
#else
    struct dirent *e;
    while ((e = readdir(d)) != NULL) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;
        return e->d_name;
    }
    return NULL;
#endif
}

static inline void os_closedir(OS_DIR *d)
{
#ifdef _WIN32
    if (d) {
        if (d->hFind != INVALID_HANDLE_VALUE) FindClose(d->hFind);
        free(d);
    }
#else
    if (d) closedir(d);
#endif
}

/* ---------------- spawn + wait (fork + exec + wait) ---------------- */

static inline int os_run_child(const char *prog, const char *arg,
                        os_pid_t *child_pid, int *exit_code)
{
#ifdef _WIN32
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    char cmdline[4096];

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (arg != NULL)
        snprintf(cmdline, sizeof(cmdline), "\"%s\" \"%s\"", prog, arg);
    else
        snprintf(cmdline, sizeof(cmdline), "\"%s\"", prog);

    if (!CreateProcessA(NULL, cmdline, NULL, NULL, FALSE, 0,
                        NULL, NULL, &si, &pi)) {
        if (child_pid) *child_pid = 0;
        return -1;
    }

    if (child_pid) *child_pid = (os_pid_t)pi.dwProcessId;
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD code = 1;
    if (!GetExitCodeProcess(pi.hProcess, &code)) code = 1;
    if (exit_code) *exit_code = (int)code;

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
#else
    pid_t pid = fork();
    if (pid < 0) return -1;
    if (pid == 0) {
        if (arg != NULL) {
            char *const av[] = { (char *)prog, (char *)arg, NULL };
            execv(prog, av);
        } else {
            char *const av[] = { (char *)prog, NULL };
            execv(prog, av);
        }
        _exit(127);
    }
    {
        int status = 0;
        waitpid(pid, &status, 0);
        if (child_pid) *child_pid = (os_pid_t)pid;
        if (exit_code) *exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
    return 0;
#endif
}

/*
 * Spawn a process from an argv array (argv[0] is the program) and wait for it.
 * Unlike system(), this never invokes a shell, so no command-line string is
 * ever parsed by a shell -> no shell-injection risk. All args are compile-time
 * constants chosen by the program itself, never raw user input.
 */
static inline int os_exec_argv(char *const argv[], int *exit_code)
{
#ifdef _WIN32
    char cmdline[8192];
    size_t n = 0;

    cmdline[0] = '\0';
    for (int i = 0; argv[i] != NULL && n < sizeof(cmdline); i++) {
        int written = snprintf(cmdline + n, sizeof(cmdline) - n,
                               "\"%s\" ", argv[i]);
        if (written < 0 || (size_t)written >= sizeof(cmdline) - n) {
            if (exit_code) *exit_code = -1;
            return -1;
        }
        n += (size_t)written;
    }

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(argv[0], cmdline, NULL, NULL, FALSE, 0,
                        NULL, NULL, &si, &pi)) {
        if (exit_code) *exit_code = -1;
        return -1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 1;
    if (!GetExitCodeProcess(pi.hProcess, &code)) code = 1;
    if (exit_code) *exit_code = (int)code;

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
#else
    pid_t pid = fork();
    if (pid < 0) return -1;
    if (pid == 0) {
        execv(argv[0], argv);
        _exit(127);
    }
    {
        int status = 0;
        waitpid(pid, &status, 0);
        if (exit_code) *exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
    return 0;
#endif
}

#endif /* OSPORT_H */
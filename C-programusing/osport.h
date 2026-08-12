#ifndef OSPORT_H
#define OSPORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Cross-platform operating-system helpers.
 *
 * On Windows (when _WIN32 is defined) the POSIX headers unistd.h,
 * sys/wait.h and dirent.h do not exist, so this header provides its own
 * implementations using the Windows APIs. On Linux/macOS it simply uses the
 * native POSIX calls. Every program in this folder only needs this header,
 * so it can be compiled on any system without include errors.
 */

#ifdef _WIN32
    #include <windows.h>
    #define OS_PATH_SEP "\\"
    #define OS_EXE_SUFFIX ".exe"
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
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

static os_pid_t os_getpid(void)
{
#ifdef _WIN32
    return GetCurrentProcessId();
#else
    return (os_pid_t)getpid();
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

static OS_DIR *os_opendir(const char *path)
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

static const char *os_readdir(OS_DIR *d)
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

static void os_closedir(OS_DIR *d)
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

static int os_run_child(const char *prog, const char *arg,
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

#endif /* OSPORT_H */
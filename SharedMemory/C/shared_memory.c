#include <stdio.h>
#include <string.h>

#define SHM_SIZE 4096
#define OFFSET_FLAG 0
#define OFFSET_LEN 4
#define OFFSET_DATA 8

#ifdef _WIN32
#include <windows.h>
#define SHM_NAME "oslab_shared_memory"
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#define SHM_NAME "/oslab_shared_memory"
#endif

static void read_message(void *mem) {
    volatile int *flag = (volatile int *)((char *)mem + OFFSET_FLAG);
    int len;

    while (*flag == 0) {                     /* spin until the parent publishes a message */
#ifdef _WIN32
        Sleep(10);
#else
        struct timespec pause = {0, 10000000};
        nanosleep(&pause, NULL);
#endif
    }

    len = *(int *)((char *)mem + OFFSET_LEN);
    printf("Child read:  %.*s\n", len, (char *)mem + OFFSET_DATA);
    fflush(stdout);
    *flag = 0;
}

static void write_message(void *mem, const char *msg) {
    int len = (int)strlen(msg);
    /* shortcut: messages longer than the region are truncated, enlarge SHM_SIZE to send more */
    if (len > SHM_SIZE - OFFSET_DATA) len = SHM_SIZE - OFFSET_DATA;
    memcpy((char *)mem + OFFSET_LEN, &len, sizeof len);
    memcpy((char *)mem + OFFSET_DATA, msg, len);
    printf("Parent wrote: %.*s\n", len, msg);
    fflush(stdout);
    *(volatile int *)((char *)mem + OFFSET_FLAG) = 1;
}

#ifdef _WIN32

int main(int argc, char **argv) {
    char msg[SHM_SIZE - OFFSET_DATA];
    HANDLE map;
    void *mem;

    if (argc > 1 && strcmp(argv[1], "child") == 0) {
        map = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, SHM_NAME);
        if (map == NULL) return 1;
        mem = MapViewOfFile(map, FILE_MAP_ALL_ACCESS, 0, 0, SHM_SIZE);
        read_message(mem);
        UnmapViewOfFile(mem);
        CloseHandle(map);
        return 0;
    }

    printf("Enter a message to send through shared memory: ");
    fflush(stdout);
    if (fgets(msg, sizeof msg, stdin) == NULL) return 1;
    msg[strcspn(msg, "\r\n")] = '\0';

    map = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SHM_SIZE, SHM_NAME);
    if (map == NULL) { fprintf(stderr, "CreateFileMapping failed\n"); return 1; }
    mem = MapViewOfFile(map, FILE_MAP_ALL_ACCESS, 0, 0, SHM_SIZE);
    memset(mem, 0, SHM_SIZE);

    {
        char exe[MAX_PATH], cmdline[MAX_PATH + 32];
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        GetModuleFileNameA(NULL, exe, MAX_PATH);
        sprintf(cmdline, "\"%s\" child", exe);
        memset(&si, 0, sizeof si);
        si.cb = sizeof si;
        if (!CreateProcessA(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            fprintf(stderr, "CreateProcess failed\n");
            return 1;
        }
        write_message(mem, msg);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }

    UnmapViewOfFile(mem);
    CloseHandle(map);
    return 0;
}

#else

int main(void) {
    char msg[SHM_SIZE - OFFSET_DATA];
    int fd;
    void *mem;
    pid_t pid;

    printf("Enter a message to send through shared memory: ");
    fflush(stdout);
    if (fgets(msg, sizeof msg, stdin) == NULL) return 1;
    msg[strcspn(msg, "\r\n")] = '\0';

    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600);
    if (fd == -1) { perror("shm_open"); return 1; }
    if (ftruncate(fd, SHM_SIZE) == -1) { perror("ftruncate"); return 1; }
    mem = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) { perror("mmap"); return 1; }
    close(fd);
    memset(mem, 0, SHM_SIZE);

    pid = fork();
    if (pid == 0) {
        read_message(mem);
        munmap(mem, SHM_SIZE);
        _exit(0);
    }

    write_message(mem, msg);
    waitpid(pid, NULL, 0);
    munmap(mem, SHM_SIZE);
    shm_unlink(SHM_NAME);
    return 0;
}

#endif

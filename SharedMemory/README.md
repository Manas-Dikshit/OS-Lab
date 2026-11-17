# Inter-Process Communication (IPC) Using Shared Memory

**Shared memory** is the fastest form of IPC: the OS maps the *same physical pages* into
two processes' address spaces, so one process can write bytes that the other reads
directly. There is no message queue and no kernel copy per byte — but also no built-in
synchronisation, so the programs add a small `flag` in the shared region to signal when a
message is ready.

Both versions do the same thing: the parent asks for a short message, spawns a child
process, writes the message into the shared region, and the child prints it back.

| File | Implementation |
|------|----------------|
| `SharedMemoryIPC.java` | Java `FileChannel.map` over a temp file; child started with `ProcessBuilder` |
| `C/shared_memory.c` | One source: Win32 `CreateFileMapping`/`MapViewOfFile` + `CreateProcess`, or POSIX `shm_open`/`mmap` + `fork` |

---

## The shared region

Both programs lay out the first 4 KiB of shared memory identically:

| Offset | Size | Contents |
|--------|------|----------|
| 0 | 4 bytes | `flag`: 0 = no message, 1 = message ready |
| 4 | 4 bytes | message length |
| 8 | up to 4088 bytes | message bytes |

The parent writes the length and bytes, flushes, then sets `flag = 1`. The child polls the
flag every 10 ms, reads the message, prints it and clears the flag. A message longer than
4088 bytes is truncated. Only one instance of a program should run at a time — both
versions use a fixed shared-memory name.

---

## Running the programs

### Java

Java has no raw `shmget`, so a memory-mapped file is used: mapping the *same file* in two
processes gives them genuinely shared pages. The sources declare `package SharedMemory;`,
so compile and run from the repo root:

```bash
javac SharedMemory/*.java
echo "Hello from shared memory" | java SharedMemory.SharedMemoryIPC
```

The parent creates (and reuses) `oslab_shared_memory.bin` in the system temp directory.
It is left there so the child can map the same path; it is only 4 KiB.

### C

On Windows, plain `gcc` works — the Win32 mapping APIs need no extra libraries:

```bash
gcc shared_memory.c -o shared_memory
shared_memory.exe
```

On Linux/macOS, `shm_open` may need `-lrt` (older glibc); `make` adds it on Linux:

```bash
make                 # or: gcc shared_memory.c -o shared_memory -lrt
./shared_memory
```

The child is a second copy of the same executable: Windows passes it the `child` argument
and opens the named mapping; POSIX `fork()` inherits the `MAP_SHARED` mapping directly.

### Sample run

Both versions print the same three lines:

```
Enter a message to send through shared memory: Parent wrote: Hello from shared memory
Child read:  Hello from shared memory
```

The two processes never pass the message through a pipe or a file read/write call — the
child sees the bytes because its address space maps the same physical memory.

---

## Why a flag is needed

If the child read the region before the parent had written it, it would see stale or
partial data. Setting `flag` **after** the payload is written (and flushing) is a
minimal form of publication: the child only reads once the flag says the message is
complete. Real systems would use a semaphore, mutex or condition variable instead of a
10 ms polling loop — that avoids burning CPU while waiting and handles many messages.

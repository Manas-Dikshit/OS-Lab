# OS Lab

An operating-systems lab collection in two halves:

1. **System-call programs** (Java) — process management, process IDs, and directory
   operations, showing how Java's standard library maps onto the underlying OS calls.
2. **Algorithm folders** — CPU scheduling, deadlock avoidance, synchronization, and page
   replacement, each implemented **twice: once in Java and once in C**, in separate
   folders, with identical input and output.

> Every `.java` file and every `.c` file is a standalone, self-contained program with no
> third-party dependencies. Compile the one you want and run it.

---

## Repo layout

```
OSLab/
├── ChildTask.java                  # child process entry point
├── ProcessSyscalls.java            # fork + exec + wait via ProcessBuilder
├── DirectorySetup.java             # opendir / readdir / closedir
├── MultipleDirectories.java        # directory listing, several directories at once
├── SyscallCommands.java            # runs ls / cp / grep and captures their output
├── newdir/                         # scratch directory used by the examples above
│
├── CPUScheduling/                  # 6 algorithms, Java at root + C/
│   ├── FCFS.java  SJF.java  SRTF.java
│   ├── PriorityScheduling.java  PreemptivePriorityScheduling.java
│   ├── RoundRobin.java
│   ├── C/{fcfs,sjf,srtf,priority,preemptive_priority,round_robin}.c
│   └── README.md
│
├── BankerAlgo/                     # deadlock avoidance, Java at root + C/
│   ├── SafetyAlgorithm.java  ResourceRequestAlgorithm.java
│   ├── C/{safety_algorithm,resource_request_algorithm}.c
│   └── README.md
│
├── ProducerConsumer/               # bounded-buffer synchronization
│   ├── ProducerConsumer.java
│   ├── C/producer_consumer.c  C/Makefile
│   └── README.md
│
├── PageReplacement/                # 6 algorithms, Java at root + C/
│   ├── FIFO.java  LRU.java  Optimal.java  LFU.java  MFU.java  Clock.java
│   ├── C/{fifo,lru,optimal,lfu,mfu,clock}.c  C/Makefile
│   └── README.md
│
└── README-<name>.md                # one per system-call program
```

---

## System-call programs

| File | What it demonstrates |
|------|----------------------|
| `ChildTask.java` | A child process entry point; prints its own PID and an argument |
| `ProcessSyscalls.java` | `fork` + `exec` + `wait` (process creation and reuse) via `ProcessBuilder` |
| `DirectorySetup.java` | `opendir`, `readdir`, `closedir` (directory listing) |
| `MultipleDirectories.java` | `opendir`, `readdir`, `closedir` across multiple directories |
| `SyscallCommands.java` | Executes shell-level system programs (`ls`, `cp`, `grep`) and captures output |

Each file has its own `README-<name>.md` that explains the system calls it uses in detail.

---

## Algorithm collections

Each algorithm folder holds the Java sources at its root and the matching C sources in a
`C/` subfolder, plus its own `README.md` describing the algorithm and the exact steps to
compile and run both versions.

| Folder | Topic |
|--------|-------|
| [`CPUScheduling/`](./CPUScheduling) | Six CPU scheduling algorithms: FCFS, SJF, SRTF, Priority, Preemptive Priority, Round Robin |
| [`BankerAlgo/`](./BankerAlgo) | Deadlock avoidance — the Safety and Resource-Request algorithms |
| [`ProducerConsumer/`](./ProducerConsumer) | Producer–consumer bounded-buffer problem solved with semaphores |
| [`PageReplacement/`](./PageReplacement) | Six page replacement algorithms: FIFO, LRU, Optimal, LFU, MFU, Clock |

To compare algorithms, run several of them over the same input and compare their metrics —
CPU scheduling reports average turnaround and waiting time, page replacement reports total
page faults and hit ratio, Banker's reports a safe sequence, producer–consumer reports
produced/consumed item order.

---

## The C programs

Every C source is ANSI C99 with no external libraries, so gcc or clang builds it, and the
same file works on Windows and Linux/macOS:

```bash
cd CPUScheduling/C
gcc fcfs.c -o fcfs       # compile
./fcfs                   # run (fcfs.exe on Windows)
```

Compile all C sources in a folder in one loop:

```bash
for f in *.c; do gcc "$f" -o "${f%.c}"; done
```

`PageReplacement/C` and `ProducerConsumer/C` ship a `Makefile` that builds everything in
one command:

```bash
cd PageReplacement/C && make && make clean
```

`ProducerConsumer/C` needs `-pthread` on Linux/macOS (its Makefile adds it automatically);
on Windows it uses a Win32 shim so plain `gcc` works with no extra libraries.

---

## Prerequisites

- **JDK 9 or later** (programs use `ProcessHandle`, added in Java 9) — verify with
  `java -version` and `javac -version`
- **A C compiler** (gcc or clang) — only for the C programs; `make` is optional
- **Git for Windows** (bash shell) — only required by `SyscallCommands.java`, which shells
  out to `ls`, `cp`, and `grep`. If you want that program to run, install Git Bash at the
  default location or update the Bash path in the source.

---

## How to compile and run

### System-call programs

Compile all five from the repo root:

```bash
javac *.java
```

This produces the corresponding `.class` files (git-ignored via the repo's `.gitignore`).
Then:

```bash
# 1. Print the current process PID
java ChildTask
java ChildTask "any message"

# 2. Parent spawns a child; shows parent PID, child PID, and the child's exit code
java ProcessSyscalls

# 3. List the contents of the current directory
java DirectorySetup

# 4. List the contents of a specific directory
java DirectorySetup C:\Users\manas\Downloads\OSLab

# 5. List the contents of several directories at once
java MultipleDirectories newdir .
java MultipleDirectories newdir C:\Users\manas\Downloads\OSLab C:\Windows

# 6. Run shell utilities (ls, cp, grep) and print their output
java SyscallCommands
```

> `ProcessSyscalls` launches `ChildTask`, so keep both `.class` files in the same folder.

> **Windows note:** Directory paths like `C:\Users\...` can be passed as-is in most shells.
> If your shell interprets backslashes, use forward slashes instead: `C:/Users/manas/Downloads/OSLab`.

### Algorithm programs

Each algorithm folder is compiled on its own — they are separate programs, not a package:

```bash
cd CPUScheduling
javac SJF.java && java SJF        # Java
cd C && gcc sjf.c -o sjf && ./sjf   # the same algorithm in C
```

```bash
cd PageReplacement
javac LRU.java && java LRU
cd C && gcc lru.c -o lru && ./lru
```

`javac *.java` or `make` inside a folder builds everything in that folder at once. Each
program asks for its input interactively — for example page replacement asks for the
frame count and the reference string; see the folder `README.md` for the exact prompts.

---

## The system calls demonstrated

| OS system call | Purpose | Demonstrated in |
|----------------|---------|-----------------|
| `fork` | Create a new child process | `ProcessSyscalls.java` |
| `exec` | Replace a process image with a new program | `ProcessSyscalls.java` |
| `wait` / `waitpid` | Parent waits for a child to finish | `ProcessSyscalls.java` |
| `getpid` | Get the current process ID | `ChildTask.java`, `ProcessSyscalls.java` |
| `exit` | Terminate a process and return a status | `ChildTask.java`, `ProcessSyscalls.java`, `SyscallCommands.java` |
| `opendir` | Open a directory for reading | `DirectorySetup.java`, `MultipleDirectories.java` |
| `readdir` | Read the next entry of an open directory | `DirectorySetup.java`, `MultipleDirectories.java` |
| `closedir` | Close an open directory | `DirectorySetup.java`, `MultipleDirectories.java` |

> Java exposes these OS concepts through its standard library rather than raw system calls:
> `ProcessBuilder` / `Process` stand in for `fork`/`exec`/`wait`, `ProcessHandle` for `getpid`,
> and `Files.newDirectoryStream` for `opendir`/`readdir`/`closedir`.

---

## Per-file system call readmes

- [`README-ChildTask.md`](./README-ChildTask.md)
- [`README-ProcessSyscalls.md`](./README-ProcessSyscalls.md)
- [`README-DirectorySetup.md`](./README-DirectorySetup.md)
- [`README-MultipleDirectories.md`](./README-MultipleDirectories.md)
- [`README-SyscallCommands.md`](./README-SyscallCommands.md)

---

## Cleanup

Remove the compiled binaries when you are done:

```bash
rm *.class                              # root system-call programs
rm CPUScheduling/*.class BankerAlgo/*.class ProducerConsumer/*.class PageReplacement/*.class
for f in */*/*.c; do rm -f "${f%.c}.exe" "${f%.c}"; done   # C binaries
make -C PageReplacement/C clean          # where a Makefile exists
```

---

## License

Free to use for learning purposes.

---

Made by MRD with ❤️

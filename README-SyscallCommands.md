# SyscallCommands

Runs several standard Unix command-line utilities (`ls`, `cp`, `grep`) from Java, reads
their output, waits for them to finish, and reports each command's exit code.

## System calls used

| System call | Purpose |
|-------------|---------|
| `fork` | Spawns a new child process for each shell command via `ProcessBuilder.start()`. |
| `exec` | Loads the program for each command (the `bash` shell, which in turn runs `ls`, `cp`, `grep`). |
| `wait` / `waitpid` | Waits for each spawned command to finish (`p.waitFor()`). |
| `read` | Reads the standard output of each child through `p.getInputStream()`. |
| `exit` | Terminates a process with a status; the shell reports the command's status as its exit code. |

## What it demonstrates

- Using an OS shell to execute system utilities.
- Capturing a command's **stdout** so a Java program can consume the result.
- Reading a process's **exit status** to know whether a command succeeded (0) or failed
  (non-zero).

## Required setup (important)

These commands are run through **Git Bash**, so you must have Git for Windows installed.
The source uses this fixed path:

```
C:\Program Files\Git\bin\bash.exe
```

If Git Bash is installed elsewhere on your machine, update that path in
`SyscallCommands.java` (lines 7-10).

## How to use

Make sure the output directory exists first, then run:

```bash
mkdir newdir
javac SyscallCommands.java
java SyscallCommands
```

What it runs, in order:

1. `ls` — lists the current directory.
2. `cp SyscallCommands.java newdir/SyscallCommandsCopy.java` — copies a file.
3. `ls newdir` — verifies the copy exists.
4. `grep class SyscallCommands.java` — searches for lines containing `class`.

Each block prints its captured output followed by `Exit code: <code>`.

## Notes

- `redirectErrorStream(true)` merges stderr into stdout so error messages appear in the
  same output stream.
- Installing Git for Windows is required; without the bash shell the commands will fail
  to start.
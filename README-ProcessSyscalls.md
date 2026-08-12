# ProcessSyscalls

Demonstrates the core Unix process-control triad: creating a process, loading a new
program into it, and waiting for it to finish.

## System calls used

| System call | Purpose |
|-------------|---------|
| `fork` | Creates a new child process as a copy of the parent. In Java this is done with `ProcessBuilder(...).start()`, which launches a separate OS process. |
| `exec` | Replaces the calling process's memory image with a brand-new program. Here `ProcessBuilder` starts a fresh JVM running `ChildTask`. |
| `wait` / `waitpid` | Blocks the parent until a child terminates and reaps its exit status. Done via `child.waitFor()`, and the result is read with `child.exitValue()`. |
| `getpid` | Returns a process's own ID, used to display the parent's PID. |
| `exit` | Terminates a process and returns a status code to its parent. |

## What it demonstrates

- The **parent** process (this program) creates a **child** process running `ChildTask`.
- Both the parent and the child PIDs are printed, showing they are distinct processes.
- The parent **waits** for the child to finish and prints its exit code.

## The flow

1. Parent records its own PID.
2. Parent spawns child (`java -cp . ChildTask "Hello from child process"`).
3. Child runs `ChildTask`, which prints its PID and the message, then exits with code `0`.
4. Parent waits for the child, then prints `Child exited with code: 0`.

## How to use

Compile both files first (the parent and the child), then run only the parent:

```bash
javac ChildTask.java ProcessSyscalls.java
java ProcessSyscalls
```

Expected output pattern:

```
Parent process PID: 12345
Child process PID: 12346
Child process PID: 12346
Child received: Hello from child process
Child exited with code: 0
```

## Notes

- The child must live on the classpath (`-cp .`) that the parent uses to start it.
- `waitFor()` returns once the child terminates, mirroring `waitpid` semantics.
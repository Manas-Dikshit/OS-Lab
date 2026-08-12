# ChildTask

A simple child-process entry point. It prints its own process ID and optionally prints
a message that was passed to it as a command-line argument, then exits.

## System calls used

| System call | Purpose |
|-------------|---------|
| `getpid` | Returns the PID of the currently running process. Used here to print the child's own PID: `ProcessHandle.current().pid()` |
| `exit` | Terminates the process with a status code. Called via `System.exit(0)` |

## What it demonstrates

- A process can obtain its own identifier.
- A process can terminate itself deliberately with an exit status.

## Notes

- `ProcessHandle.current()` returns a handle to the running Java process, and `.pid()`
  gives its OS-level process ID.
- Running this program standalone prints the PID of whatever JVM you run it in.

## How to use

```bash
javac ChildTask.java
java ChildTask
java ChildTask "Hello from child process"
```
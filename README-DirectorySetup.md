# DirectorySetup

Lists the contents of a directory, simulating the classic Unix directory-reading API.

## System calls used

| System call | Purpose |
|-------------|---------|
| `opendir` | Opens a directory for reading. In Java this is the opening of a `DirectoryStream`. |
| `readdir` | Reads the next entry from the open directory on each iteration. |
| `closedir` | Closes the directory when done, releasing the handle and OS resources. |

## What it demonstrates

- The standard **open / read / close** lifecycle applied to directories:
  open the directory, iterate over its entries, then close it.
- Java's `Files.newDirectoryStream` is wrapped in a *try-with-resources* statement so the
  underlying directory stream is **closed automatically**, even if an error occurs.

## How to use

```bash
javac DirectorySetup.java
java DirectorySetup            # lists the current directory (default is ".")
java DirectorySetup newdir     # lists a specific directory
java DirectorySetup C:\Users\manas\Downloads\OSLab
```

## Notes

- If no argument is given, the program defaults to listing `.` (the current directory).
- A failed open (e.g. the path does not exist) is reported without crashing the program.
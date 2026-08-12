# MultipleDirectories

Extends the single-directory listing to handle **several directories** in one run,
applying the same directory system calls to each path given on the command line.

## System calls used

| System call | Purpose |
|-------------|---------|
| `opendir` | Opens each supplied directory for reading. |
| `readdir` | Reads the entries of each opened directory. |
| `closedir` | Closes each directory stream when its listing finishes (or on error). |

## What it demonstrates

- Reusing a single listing routine across multiple resources.
- **Per-directory error isolation:** if one directory cannot be opened, the error is
  reported and the program continues with the remaining directories rather than aborting.

## How to use

```bash
javac MultipleDirectories.java
java MultipleDirectories newdir .
java MultipleDirectories newdir C:\Users\manas\Downloads\OSLab C:\Windows
```

## Notes

- At least one directory argument is required; otherwise the program prints usage help:
  `Usage: java MultipleDirectories <dir1> <dir2> ...`
- Each directory is opened and closed independently via try-with-resources.
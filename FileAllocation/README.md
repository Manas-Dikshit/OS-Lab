# File Allocation Methods

Files live on disk in fixed-size **blocks**. The allocation method decides *which* blocks
a file gets and *how the OS finds the next one*. The three classic choices are
**sequential (contiguous)**, **linked**, and **indexed** allocation.

This folder contains all three, each implemented in **Java and C** with identical input and
output.

| Method | Java | C | Block lookup |
|--------|------|---|--------------|
| Sequential (contiguous) | `SequentialAllocation.java` | `C/sequential.c` | start block + offset, arithmetic only |
| Linked | `LinkedAllocation.java` | `C/linked.c` | each block stores a pointer to the next |
| Indexed | `IndexedAllocation.java` | `C/indexed.c` | one index block stores all the pointers |

---

## How each method works

### 1. Sequential (contiguous) allocation

A file occupies one unbroken run of blocks. The directory entry needs only a **start
block** and a **length**.

- **Good:** fastest — reading block *k* of a file is one addition, and sequential reads
  are physically sequential on the disk.
- **Bad:** needs a big enough hole up front, and files cannot grow in place. Free space
  gets chopped into small holes (**external fragmentation**); compaction is the usual fix.

This program uses **first fit**: it takes the first free run long enough for the file.

### 2. Linked allocation

Each file is a chain of blocks scattered anywhere on disk; every block holds a pointer to
the next one. The directory entry stores the first block's address.

- **Good:** no external fragmentation — any free block works, and files can grow by
  appending another free block.
- **Bad:** no random access — reaching block *k* means following *k* pointers. Pointers
  also consume a few bytes of every block.

### 3. Indexed allocation

Each file gets an **index block** whose contents are the addresses of all its data blocks.
The directory entry stores the index block's address.

- **Good:** supports random access *and* uses scattered free blocks, so no external
  fragmentation.
- **Bad:** costs one extra block per file, which is wasteful for tiny files. A file that
  outgrows one index block needs linked index blocks or a multi-level index.

In all three programs the lowest-numbered suitable free blocks are used, files are placed
in input order, and there is no deletion or growth after placement.

---

## Running the programs

```
FileAllocation/
├── SequentialAllocation.java
├── LinkedAllocation.java
├── IndexedAllocation.java
└── C/
    ├── Makefile
    ├── sequential.c
    ├── linked.c
    └── indexed.c
```

### Java

The sources declare `package FileAllocation;`, so compile and run from the repo root:

```bash
javac FileAllocation/*.java
java FileAllocation.SequentialAllocation
java FileAllocation.LinkedAllocation
java FileAllocation.IndexedAllocation
```

### C

ANSI C99, no extra libraries. Build any one directly, or use the Makefile:

```bash
gcc sequential.c -o sequential && ./sequential   # Windows: sequential.exe
make                                              # builds all three
make clean
```

### Input format

All six programs ask the same questions. A size is simply the number of blocks the file
needs:

```
Enter number of disk blocks: 12
Enter number of files: 4
Enter name and size for file 1 (e.g. alpha 4): alpha 4
Enter name and size for file 2 (e.g. alpha 4): beta 3
Enter name and size for file 3 (e.g. alpha 4): gamma 6
Enter name and size for file 4 (e.g. alpha 4): delta 2
```

### Sample output — Sequential

```
File	Size	Start	Blocks
alpha	4	0	0,1,2,3
beta	3	4	4,5,6
gamma	6	Not allocated	-
delta	2	7	7,8

Free blocks: 3
```

`gamma` needs six consecutive blocks but only five are left together (blocks 7–11) —
external fragmentation, even though three of those blocks are free.

### Sample output — Linked

```
File	Size	Chain
alpha	4	0->1->2->3
beta	3	4->5->6
gamma	6	Not allocated
delta	2	7->8

Free blocks: 3
```

Blocks need not be adjacent, but there are still only 12 blocks total, so `gamma` and the
free-space count end up the same as above.

### Sample output — Indexed

```
File	Size	Index	Data blocks
alpha	4	0	1->2->3->4
beta	3	5	6->7->8
gamma	6	-	Not allocated
delta	2	9	10->11

Free blocks: 0
```

Here `delta` gets block 9 as its index and blocks 10–11 as data. Each file pays one extra
block for its index, so `gamma` still fails: six data blocks plus an index cannot fit in
the three blocks left after `beta`.

### Comparing the methods

| | Sequential | Linked | Indexed |
|---|------------|--------|---------|
| Random access | Yes | No | Yes |
| External fragmentation | Yes | No | No |
| Extra space per file | None | 1 pointer per block | 1 whole block |
| Grows easily | No | Yes | Yes |

Run all three on the same input and compare which files get allocated and how many blocks
are left over.

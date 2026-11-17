# Memory Allocation for Fixed Partitions

In **fixed partitioning**, main memory is carved up once into a set of partitions of fixed
size. Each incoming process needs one contiguous partition, and a process can only go into
a partition at least as big as itself. When several partitions could work, the OS needs a
placement rule — the three classic ones are **First Fit**, **Best Fit**, and **Worst Fit**.

This folder contains all three, each implemented in **Java and C** with identical input and
output.

| Algorithm | Java | C | Picks the... |
|-----------|------|---|--------------|
| First Fit | `FirstFit.java` | `C/first_fit.c` | first free partition large enough |
| Best Fit | `BestFit.java` | `C/best_fit.c` | free partition with the smallest leftover that still fits |
| Worst Fit | `WorstFit.java` | `C/worst_fit.c` | free partition with the largest leftover |

---

## The data

- **Partitions** — fixed block sizes, numbered from 1 in the order entered. A partition
  holds exactly one process at a time.
- **Processes** — memory requests, processed in the order entered.

A request that fits leaves `partition size - process size` unused inside that partition.
That unused space is **internal fragmentation**. A process that fits in no free partition
is **not allocated** — with fixed partitions, the free space it cannot use is
**external fragmentation**.

The programs print one row per process with the partition it landed in and the space
wasted there, then the allocation count, total internal fragmentation, and the number of
processes left out.

---

## How each algorithm picks a partition

### 1. First Fit

Scan partitions from the start and take the first free one that is large enough.

- **Good:** fastest of the three — it stops at the first match, and it tends to cluster
  allocations at the low-address end, leaving big partitions free at the high end.
- **Bad:** that clustering can use up small partitions early and then force a large
  process to wait even though enough total memory is free.

### 2. Best Fit

Scan **all** free partitions that fit and take the one with the smallest leftover.

- **Good:** minimises wasted space in the partition it chooses, so small requests are kept
  out of large partitions.
- **Bad:** scans every partition, and the tiny leftover fragments it creates are often
  useless to later processes. In practice it does not always beat First Fit overall.

### 3. Worst Fit

Scan all free partitions that fit and take the one with the largest leftover.

- **Good:** leaves a large hole behind, so the leftover may still satisfy another process.
- **Bad:** chews through big partitions quickly and can strand a late large process.
  Usually performs worst of the three and is mostly a teaching counterexample.

Ties (equal leftover sizes) go to the lowest-numbered partition in both Best Fit and
Worst Fit, since comparisons are strict.

---

## Running the programs

```
MemoryAllocation/
├── FirstFit.java
├── BestFit.java
├── WorstFit.java
└── C/
    ├── Makefile
    ├── first_fit.c
    ├── best_fit.c
    └── worst_fit.c
```

### Java

Plain Java console programs, no dependencies. Java SE 8+ required. The sources declare
`package MemoryAllocation;`, so compile and run them from the repo root (one level above
this folder):

```bash
javac MemoryAllocation/*.java    # compile all three
java MemoryAllocation.FirstFit   # run
java MemoryAllocation.BestFit
java MemoryAllocation.WorstFit
```

### C

ANSI C99, no extra libraries. Any C compiler works (gcc, clang, etc.).

```bash
gcc first_fit.c -o first_fit   # compile
./first_fit                    # Linux / macOS (or first_fit.exe on Windows)
```

Or build every C program at once:

```bash
cd C
make               # all three
make clean         # remove the binaries
```

### Input format

All six programs ask the same things. Sizes are plain integers in whatever unit you want
(KB, MB, words — the programs are unit-agnostic):

```
Enter number of partitions: 5
Enter size of partition 1: 100
Enter size of partition 2: 500
Enter size of partition 3: 200
Enter size of partition 4: 300
Enter size of partition 5: 600
Enter number of processes: 4
Enter size of process 1: 212
Enter size of process 2: 417
Enter size of process 3: 112
Enter size of process 4: 426
```

Output is the allocation table followed by the summary:

```
Process	Size	Partition	Wasted
1	212	2	288
2	417	5	183
3	112	3	88
4	426	Not allocated	-

Allocated processes: 3/4
Total internal fragmentation: 559
Processes not allocated: 1
```

That example is First Fit. The Java and C versions of a given algorithm print identical
output for the same input.

### Expected results

Same input as above, run through each algorithm:

| Algorithm | Allocated | Internal fragmentation | Not allocated |
|-----------|-----------|------------------------|---------------|
| First Fit | 3/4 | 559 | 1 |
| Best Fit | 4/4 | 433 | 0 |
| Worst Fit | 3/4 | 659 | 1 |

Best Fit is the only one that places all four processes here and leaves the least wasted
space. Worst Fit places the 212-request in the 600 partition, which blocks the final
426-request — a good illustration of why "leave the biggest hole" is rarely the best rule.
Run all three on your own inputs and compare the three summary lines.

# Page Replacement Algorithms

When a process references a page that is **not** in memory, a **page fault** occurs and
the OS must free a frame to load it. Which frame gets thrown out is the page replacement
algorithm — and the one that produces the fewest faults is the one that avoids the most
expensive disk I/O.

This folder contains six classic algorithms, each implemented in **Java and C**, with
identical input and output.

| Algorithm | Java | C | Replaces the page that was... |
|-----------|------|---|-------------------------------|
| FIFO — First In First Out | `FIFO.java` | `C/fifo.c` | loaded longest ago |
| LRU — Least Recently Used | `LRU.java` | `C/lru.c` | used longest ago |
| Optimal (OPT / MIN) | `Optimal.java` | `C/optimal.c` | needed farthest in the future |
| LFU — Least Frequently Used | `LFU.java` | `C/lfu.c` | referenced least often |
| MFU — Most Frequently Used | `MFU.java` | `C/mfu.c` | referenced most often |
| Clock (Second Chance) | `Clock.java` | `C/clock.c` | first one found with a clear bit |

---

## The data

- **Frame count** — how many pages fit in physical memory.
- **Reference string** — the sequence of page numbers the process asks for, in order.

A **hit** is a reference to a page already in a frame; anything else is a **fault**. The
programs print the frame contents after every reference, then the total faults and the
hit ratio `(references - faults) / references`.

---

## How each algorithm picks a victim

### 1. FIFO (First In First Out)

A queue of frames in load order. A full memory means evicting the frame at the front of
the queue and pushing the new page to the back.

- **Simplicity:** the cheapest algorithm — one pointer, no per-frame state.
- **Downside:** ignores which pages are actually being used, so a long-lived page that
  arrived first is thrown out even while hot pages are missing.
- **Downside:** suffers **Belady's anomaly** — more frames can mean *more* faults.

### 2. LRU (Least Recently Used)

Each frame stores the timestamp of its last use. On a fault, evict the frame with the
oldest timestamp.

- **Good:** approximates future use well; never suffers Belady's anomaly, and is the
  usual benchmark the others are judged against.
- **Downside:** needs a timestamp updated on every access, or an approximation of
  recency (e.g. periodic shifting).
- **Practical note:** true LRU is expensive, so real systems approximate it — that is
  exactly what **Clock** does.

### 3. Optimal (OPT / MIN)

Requires knowing the whole reference string in advance. Evict the frame whose page is
needed **farthest in the future**; a page that is never referenced again is the best
victim of all.

- **Optimality:** produces the **minimum possible** number of faults. Used as the
  benchmark, not in real systems — it needs future knowledge.
- Implemented in O(frames × n): for each fault, scan forward to find each resident
  page's next use.

### 4. LFU (Least Frequently Used)

Each frame keeps a reference counter. On a fault, evict the frame with the lowest count.

- **Good:** a page used a lot stays cached; good for programs with skewed access.
- **Downside:** favours **old** pages — a page used heavily early on can block a page
  needed now, called **cache pollution**. Counter ties break toward the least recently
  used, which keeps it usable.
- **Downside:** counts must be decayed over time or they become meaningless.

### 5. MFU (Most Frequently Used)

The mirror image of LFU: evict the frame with the **highest** count (ties → least
recently used).

- **Rationale:** throw out the page that will probably be needed again soonest, on the
  assumption that a frequently used page has a lower chance of staying needed for long.
- Mostly a teaching counterexample — it performs worse than LFU in practice.

### 6. Clock (Second Chance)

Also called **Round Robin** replacement. The frames are viewed as a ring; a **hand**
points at the next candidate. Every frame has a **reference bit**, set on every hit. On
a fault the hand advances, clearing bits it meets, until it finds a frame with a clear
bit — that frame is replaced and its bit set again. The cleared frames get one more
chance if referenced again, hence "second chance".

- **Good:** approximates LRU with only one bit of state per frame and O(1) work per
  reference — cheap enough for real hardware. **Enhanced Clock** additionally prefers
  clearing-bit frames with a clean *modify* bit.

---

## Running the Programs

```
PageReplacement/
├── FIFO.java
├── LRU.java
├── LFU.java
├── MFU.java
├── Optimal.java
├── Clock.java
└── C/
    ├── Makefile
    ├── fifo.c
    ├── lru.c
    ├── lfu.c
    ├── mfu.c
    ├── optimal.c
    └── clock.c
```

### Java

Plain Java console programs, no dependencies. Java SE 8+ required.

```bash
javac LRU.java     # compile
java LRU           # run
```

### C

ANSI C, no extra libraries. Any C compiler works (gcc, clang, etc.).

```bash
gcc lru.c -o lru   # compile
./lru              # Linux / macOS (or lru.exe on Windows)
```

Or build every C program at once:

```bash
cd C
make               # all six
make clean         # remove the binaries
```

### Input Format

All twelve programs ask the same three things:

```
Enter number of frames: 3
Enter number of page references: 12
Enter the reference string: 7 0 1 2 0 3 0 4 3 0 3 2
```

Output is the frame table (one row per reference, `-1` = empty frame) followed by the
fault count and hit ratio:

```
Page	F1	F2	F3
7	7	-1	-1
0	7	0	-1
1	7	0	1
2	2	0	1
0	2	0	1
3	2	3	1
0	2	3	0
4	4	3	0
3	4	3	0
0	4	3	0
3	4	3	0
2	4	2	0

Total page faults: 8
Hit ratio: 33.33%
```

### Expected results

Same reference string, 3 frames — the Java and C versions print identical numbers:

| Algorithm | Faults | Hit ratio |
|-----------|--------|-----------|
| FIFO | 8 | 33.33% |
| LRU | 7 | 41.67% |
| Optimal | 7 | 41.67% |
| LFU | 7 | 41.67% |
| MFU | 8 | 33.33% |
| Clock | 8 | 33.33% |

Reference string `1 2 3 4 1 2 5 1 2 3 4 5` with 3 frames gives FIFO 9, LRU 10, LFU 10,
MFU 9, Clock 10, Optimal 7 — here FIFO beats LRU, which is Belady's anomaly in action.

To compare algorithms, run them all over the same string and frame count, then compare
fault counts — fewer faults means fewer disk accesses means faster execution.

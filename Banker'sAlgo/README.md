# Banker's Algorithm (Deadlock Avoidance)

Banker's algorithm prevents deadlock by only granting a resource request when the
resulting state is **safe**: there must exist an order in which every process can still
finish. It is the classic **deadlock-avoidance** algorithm.

This folder contains both halves of the algorithm, in Java and C:

| File | Algorithm | What it does |
|------|-----------|--------------|
| `SafetyAlgorithm.java` | Safety algorithm | Tells you whether a given state is safe and, if so, prints a safe sequence |
| `ResourceRequestAlgorithm.java` | Resource-request algorithm | Decides whether a single request can be granted without leaving the system unsafe |
| `C/safety_algorithm.c` | Safety algorithm | C equivalent of the above |
| `C/resource_request_algorithm.c` | Resource-request algorithm | C equivalent of the above |

---

## The data

Each process declares its maximum demand in a `Max` matrix. What it already holds is
`Allocation`. The remaining resources it may still request make up the `Need` matrix:

```
Need[i][j] = Max[i][j] - Allocation[i][j]
```

The resources still free in the system are the `Available` vector.

## 1. Safety algorithm

Tests whether a state is safe:

1. `Work = Available`, `Finish[i] = false` for every process.
2. Find an unfinished process `Pi` whose `Need[i] <= Work`. If none exists, the state is
   **unsafe**.
3. Pretend `Pi` finishes: `Work += Allocation[i]`, mark `Finish[i] = true`, record it in
   the safe sequence.
4. Repeat step 2 until every process is finished (safe) or no process is found (unsafe).

## 2. Resource-request algorithm

Decides whether to grant `Request` from process `Pi`:

1. If `Request > Need[i]`, the process exceeded its declaration — error, terminate it.
2. If `Request > Available`, the process must wait for resources.
3. Otherwise tentatively grant it:
   `Available -= Request`, `Allocation[i] += Request`, `Need[i] -= Request`.
4. Run the safety algorithm on the new state. If it is **safe**, the request is granted.
   If it is **unsafe**, roll the state back and deny the request — the process waits.

---

## Running the programs

### Java

```bash
javac SafetyAlgorithm.java ResourceRequestAlgorithm.java   # compile
java SafetyAlgorithm                                       # safe-state check
java ResourceRequestAlgorithm                              # grant-or-deny one request
```

### C

```bash
gcc safety_algorithm.c -o safety_algorithm
gcc resource_request_algorithm.c -o resource_request_algorithm
./safety_algorithm
./resource_request_algorithm
```

### Input format

Both programs prompt for the same data. Example session (one request check):

```
Enter number of processes: 5
Enter number of resource types: 3
Enter available instances of each resource: 3 3 2
Enter max demand matrix (Max):
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
Enter allocation matrix (Allocation):
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
Enter process making request (1-5): 2
Enter request vector: 1 0 2
```

The classic textbook outcome for this data: the initial state is safe with sequence
`P2 -> P4 -> P1 -> P3 -> P5`, and P1's request `(1,0,2)` is granted.
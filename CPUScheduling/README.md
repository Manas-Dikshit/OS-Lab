# CPU Scheduling Algorithms

This folder contains Java implementations of six classic CPU scheduling algorithms
— three non-preemptive and three preemptive.

| File | Algorithm | Type |
|------|-----------|------|
| `FCFS.java` | First Come First Serve | Non-Preemptive |
| `SJF.java` | Shortest Job First | Non-Preemptive |
| `PriorityScheduling.java` | Priority Scheduling | Non-Preemptive |
| `SRTF.java` | Shortest Remaining Time First | Preemptive |
| `PreemptivePriorityScheduling.java` | Preemptive Priority Scheduling | Preemptive |
| `RoundRobin.java` | Round Robin | Preemptive |

---

## Common Metrics

Every program reports the same metrics per process and their averages:

- **Arrival Time (AT)** — time the process enters the ready queue.
- **Burst Time (BT)** — CPU time the process needs to complete.
- **Completion Time (CT)** — time the process finishes execution.
- **Turnaround Time (TAT)** — `CT - AT`, total time from arrival to completion.
- **Waiting Time (WT)** — `TAT - BT`, time spent waiting in the ready queue.

---

## Non-Preemptive Algorithms

In non-preemptive scheduling, once a process is given the CPU it runs to
completion. The scheduler only picks a new process when the current one ends.

### 1. First Come First Serve (FCFS)

Non-preemptive. The process that arrives earliest runs first. Arrival order
is preserved. If the CPU is idle but the next process hasn't arrived yet, the
clock advances to its arrival time.

- **Simplicity:** trivial to implement (a FIFO queue).
- **Fairness:** no starvation — every process eventually runs.
- **Downside:** convoy effect — one long process blocks all shorter ones
  behind it, producing poor average waiting time.

### 2. Shortest Job First (SJF)

Non-preemptive. Among all arrived processes, the one with the smallest burst
time runs next. On a tie, the first arrived goes first.

- **Optimality:** gives the minimum average waiting time among all
  non-preemptive algorithms.
- **Downside:** **starvation** — a long process can wait forever if shorter
  processes keep arriving.
- **Requirement:** burst times must be known in advance (only possible for
  batch jobs).

### 3. Priority Scheduling (Non-Preemptive)

Non-preemptive. Among all arrived processes, the one with the highest priority
(running the code, **lower number = higher priority**) runs next.

- **Usage:** reflects real-world importance (system vs. user processes).
- **Downside:** **starvation** of low-priority processes and possible
  **priority inversion**. Starvation is often solved with *aging* — gradually
  raising the priority of waiting processes.

---

## Preemptive Algorithms

In preemptive scheduling, the running process can be taken off the CPU when a
better candidate arrives. This improves responsiveness but adds context-switch
overhead.

### 4. Shortest Remaining Time First (SRTF)

Preemptive version of SJF. The scheduler runs the arrived process with the
smallest **remaining** burst time. A newly arrived shorter process preempts the
running one (the clock ticks one unit at a time and re-evaluates).

- **Optimality:** gives the minimum average waiting time of all algorithms
  presented here.
- **Downside:** still suffers **starvation** for long processes, and frequent
  preemption increases context-switch overhead.

### 5. Preemptive Priority Scheduling

Preemptive version of priority scheduling. The scheduler runs the arrived
process with the highest priority; a higher-priority process that arrives
mid-run preempts the current one immediately.

- **Usage:** the basis of most real time-slicing operating systems.
- **Downside:** even worse **starvation** and **priority inversion** than the
  non-preemptive version. Aging is commonly applied here.

### 6. Round Robin (RR)

Preemptive time-slicing. Processes are kept in a FIFO queue. Each process gets
the CPU for at most one **time quantum**; if it isn't done, it is moved to the
back of the queue. Newly arrived processes join the tail of the queue.

- **Fairness:** every process gets a share — no starvation.
- **Interactivity:** best responsiveness suitable for time-sharing systems.
- **Downside:** quantum too small → too many context switches; quantum too
  large → it degrades into FCFS.

---

## Running the Programs

These are plain Java console programs with no dependencies.

```bash
javac FCFS.java        # compile
java FCFS              # run
```

Each program prompts for the number of processes, then per-process data
(arrival time, burst time, and priority where relevant). `RoundRobin.java`
also asks for the time quantum. Output is a scheduling table plus average
turnaround and waiting times.
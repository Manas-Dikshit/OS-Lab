# Producer–Consumer Problem (Semaphores)

A classic **bounded-buffer synchronization** problem. One or more producer threads place
items into a shared, fixed-size buffer while one or more consumer threads take them out.
Without synchronization the threads can corrupt the buffer, and with the wrong
synchronization they can deadlock.

The solution uses three counting semaphores over a circular buffer:

| Semaphore | Initial value | Purpose |
|-----------|---------------|---------|
| `empty` | buffer size | number of free slots left |
| `full` | 0 | number of occupied slots |
| `mutex` | 1 | protects access to the buffer itself |

Producer flow: `wait(empty)` → `wait(mutex)` → add item → `signal(mutex)` → `signal(full)`.
Consumer flow: `wait(full)` → `wait(mutex)` → remove item → `signal(mutex)` → `signal(empty)`.

The semaphores guarantee the producer never writes into a full buffer, the consumer never
reads from an empty one, and the buffer is never accessed by both threads at once.

These programs simply run a fixed batch: they ask how many items to produce, run one
producer and one consumer thread against the same buffer, and finish when the batch is done.

| File | Implementation |
|------|----------------|
| `ProducerConsumer.java` | Hand-rolled counting semaphore built on `wait`/`notify`, using `Thread` |
| `C/producer_consumer.c` | POSIX `sem_t` and `pthread` threads |

---

## Running the programs

### Java

```bash
javac ProducerConsumer.java
java ProducerConsumer
```

### C

```bash
gcc producer_consumer.c -o producer_consumer -pthread
./producer_consumer
```

> **Note:** the C program is POSIX code (`pthread.h`, `semaphore.h`), so it builds and runs
> on Linux/macOS (or WSL). The old MinGW gcc on this Windows box ships no winpthreads and
> cannot compile it.

### Sample run (10 items)

```
Enter number of items to produce: 10
Produced: 0 at slot 0
Produced: 1 at slot 1
...
Consumed: 4 from slot 4
Consumed: 5 from slot 0
```

The exact interleaving of the produced/consumed lines changes run to run — that is the
concurrency. What never changes is that every item is produced exactly once and consumed
exactly once, in order, without the buffer ever overflowing or being read empty.
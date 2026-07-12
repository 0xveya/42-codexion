_This project has been created as part of the 42 curriculum by sfurst._

# Codexion

Codexion is a POSIX-threaded resource scheduling simulation. Several coders sit
around a shared table and repeatedly need two neighboring USB dongles before
they can compile. If a coder does not begin compiling before their burnout
deadline, the simulation stops.

This README is intentionally still a working stub. It contains the sections
required by the subject and the implementation notes I want to keep visible
during peer review.

## Description

The program models the classic shared-resource problem with project-specific
rules:

- one thread per coder;
- one dongle between each neighboring pair of coders;
- each dongle is protected by its own mutex and condition variable;
- each dongle owns a heap-backed wait queue;
- the scheduler is selected at runtime: `fifo` or `edf`;
- released dongles observe a cooldown before they can be acquired again;
- a monitor thread detects burnout and stops the simulation.

The executable is named `codexion`.

```text
                    monitor thread
                         |
                         v
                +----------------+
                | app stop state |
                +----------------+
                 ^      ^      ^
                 |      |      |
      coder 1 ---+      |      +--- coder N
          \             |             /
           \            v            /
            +---- dongle queues ----+

Each coder asks its two neighboring dongles for access.
Each dongle serves its heap according to FIFO or EDF.
```

## Instructions

Build:

```bash
make
```

Run:

```bash
./codexion number_of_coders time_to_burnout time_to_compile \
  time_to_debug time_to_refactor number_of_compiles_required \
  dongle_cooldown scheduler
```

Example:

```bash
./codexion 5 3000 100 100 100 10 10 fifo
./codexion 5 3000 100 100 100 10 10 edf
```

Clean:

```bash
make clean
make fclean
make re
```

ThreadSanitizer build:

```bash
make tsan
./codexion_tsan 5 2000 100 100 100 10 10 edf
```

## Blocking Cases Handled

Deadlock prevention:

- Coders acquire dongles through per-dongle queues instead of blindly locking
  both dongles in arbitrary order.
- The single-coder case is handled separately, so one coder takes one dongle and
  then burns out cleanly instead of waiting forever for a non-existent second
  dongle.

Starvation prevention:

- FIFO uses insertion sequence numbers.
- EDF compares `last_compile_start + time_to_burnout`, with sequence numbers as
  the deterministic tie-breaker.

Cooldown handling:

- A dongle records `released_at`.
- Waiters use timed condition waits until the cooldown expires or a stop signal
  arrives.

Startup and shutdown barriers:

- Coder and monitor threads wait on `start_cond`.
- `set_stop()` and `stop_for_burnout()` broadcast both `stop_cond` and
  `start_cond`, so startup failures do not leave already-created threads stuck
  behind the start barrier.

Burnout and log shutdown:

- The monitor checks deadlines and calls `stop_for_burnout()`.
- Burnout logging is forced while holding `log_mutex`, so no normal message can
  print after the burnout line.

## Thread Synchronization Mechanisms

`state_mutex` protects global simulation state:

- `simulation_stop`;
- `simulation_started`;
- `start_time`;
- `last_compile_start`;
- `compiles_done`.

`log_mutex` serializes writes to stdout. Normal logs first check the stop flag
under `state_mutex`, then write under `log_mutex`. Burnout logs take the stop
decision and output ordering seriously: the code sets `simulation_stop`, wakes
waiters, and writes the burnout line while preventing interleaved output.

Each `t_dongle` has:

- `mutex` for `available`, `released_at`, `next_sequence`, and the heap queue;
- `cond` for waiters that need to wake on release, cooldown expiry, queue
  changes, or simulation stop.

`start_cond` is the startup gate. Threads are created first, then
`start_all_threads()` records one shared start timestamp, initializes the first
deadline for every coder, and broadcasts the start condition.

Lock contracts are documented directly in the C files with short `Lock:`
comments above functions.

## Logging Notes

The log path avoids `printf` in the hot path. It builds each line into a small
stack buffer and writes it with one `write(1, ...)` call while `log_mutex` is
held.

The funny-looking digit map in `digits_lookup()` is a compact lookup table:

```text
"000102030405...9899"
```

When printing a number, the logger peels two decimal digits at a time. The pair
`42` is read from `digits[84]` and `digits[85]`. This avoids repeated division
and modulo by 10 and keeps log formatting deterministic and allocation-free.

## Function Map

```text
main
  parse_arguments
  init_simulation
    init_mutexes
    init_dongles -> init_heap
    init_coders
    start condition setup
  start_simulation
    monitor_routine
    coder_routine
      acquire_both_dongles
        acquire_dongle
          heap_push / heap_pop / heap_remove
      compile_once
      release_both_dongles
  join_simulation
  cleanup_app
```


## Resources

### POSIX Threads

* [`pthread_create(3)`](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
* [`pthread_join(3)`](https://man7.org/linux/man-pages/man3/pthread_join.3.html)
* [`pthread_mutex_lock(3)` / `pthread_mutex_unlock(3)`](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
* [`pthread_cond_wait(3)` / `pthread_cond_signal(3)`](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html)
* [POSIX Condition Variables (The Open Group)](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_wait.html)

### Scheduling Algorithms

* [FIFO Scheduling (First-In, First-Out)](https://en.wikipedia.org/wiki/FIFO_%28computing_and_electronics%29)
* [Earliest Deadline First (EDF) Scheduling](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)

### Data Structures

* [Binary Heap](https://en.wikipedia.org/wiki/Binary_heap)
* [Heap (Data Structure)](https://en.wikipedia.org/wiki/Heap_%28data_structure%29)

### Concurrency & Synchronization

* [Monitor (Synchronization)](https://en.wikipedia.org/wiki/Monitor_%28synchronization%29)
* [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
* [Deadlock and Coffman Conditions](https://en.wikipedia.org/wiki/Deadlock#Necessary_conditions)


AI was used as a review and testing assistant for:

- designing stress and parser test cases;
- drafting local JSON-driven test fixtures;
- explaining synchronization choices in documentation.
- fix anoying norm errors for me.

The code remains my responsibility; generated suggestions were checked against
the subject and local tests before use.

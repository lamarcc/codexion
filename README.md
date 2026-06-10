*This project has been created as part of the 42 curriculum by celamarc.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the classic Dining Philosophers problem. A group of coders sit around a shared Quantum Compiler. To compile their quantum code, each coder needs to grab two USB dongles simultaneously — one on their left, one on their right. Between compilations, coders go through a debug and refactor cycle before trying to compile again.

The challenge: coders must never burn out (i.e., go too long without compiling), dongles have a cooldown after being released, and access to dongles is governed by a scheduling policy (FIFO or EDF). The simulation stops either when a coder burns out or when every coder has compiled enough times.

The project is implemented in C using POSIX threads (`pthread`), mutexes, and condition variables.

## Instructions

### Compilation

```bash
make
```

### Usage

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory. The `scheduler` argument must be either `fifo` or `edf`.

### Arguments

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (= number of dongles) |
| `time_to_burnout` | Time in ms before a coder burns out without compiling |
| `time_to_compile` | Time in ms a coder spends compiling (holds 2 dongles) |
| `time_to_debug` | Time in ms a coder spends debugging |
| `time_to_refactor` | Time in ms a coder spends refactoring |
| `number_of_compiles_required` | Simulation stops when all coders reach this count |
| `dongle_cooldown` | Time in ms a dongle is unavailable after being released |
| `scheduler` | Arbitration policy: `fifo` or `edf` |

### Example

```bash
./codexion 4 800 200 200 200 5 50 fifo
```

### Expected log format

```
0 1 has taken a dongle
2 1 has taken a dongle
2 1 is compiling
202 1 is debugging
402 1 is refactoring
...
1505 4 burned out
```

### Cleanup

```bash
make clean   # removes object files
make fclean  # removes object files and binary
make re      # full rebuild
```

## Blocking cases handled

**Deadlock prevention**
Each coder must hold two dongles to compile. A naive implementation where each coder grabs their left dongle then waits for the right one would cause a circular deadlock (each coder holding one dongle, waiting on their neighbor's). This is addressed by using a centralized dongle arbitration system: a coder only acquires a dongle once both are available according to the scheduler, preventing the circular hold-and-wait condition described by Coffman.

**Starvation prevention**
With FIFO scheduling, each dongle request is served in arrival order, guaranteeing that no coder waits indefinitely. With EDF, the coder closest to burning out is prioritized, which also prevents starvation as long as the simulation parameters are feasible.

**Dongle cooldown**
After a coder releases a dongle, it enters a cooldown period (`dongle_cooldown` ms) during which it cannot be taken. This is tracked per dongle with a timestamp verification and enforced before any grant.

**Precise burnout detection**
A dedicated monitor thread continuously checks each coder's last compile timestamp against `time_to_burnout`. When a burnout is detected, the monitor logs it and sets the `end_simulation` flag. The burnout message is guaranteed to appear within 10 ms of the actual event.

**Log serialization**
All log output is protected by a dedicated mutex. This ensures that no two state messages ever interleave on stdout, regardless of concurrent thread activity.

## Thread synchronization mechanisms

**`pthread_mutex_t`**
Used in multiple places:
- One mutex per dongle to protect its state (available, owner, cooldown timestamp, waiting queue).
- One mutex per coder to protect its information (previous_compile, nearest dongle address).
- One global mutex to protect the `end_simulation` flag read/written by both the monitor and coder threads.

**`pthread_cond_t`**
Each dongle has an associated condition variable. When a coder cannot acquire a dongle (it's in use or in cooldown), it registers its request in the dongle's priority queue and calls `pthread_cond_wait`. When a dongle becomes available, the owning thread calls `pthread_cond_broadcast` to wake up all waiting coders; each one then re-checks whether it's the next in line according to the scheduler.

**Priority queue (heap)**
Each dongle maintains a min-heap of pending requests. Under FIFO, the key is the arrival timestamp. Under EDF, the key is `last_compile_start + time_to_burnout`. This ensures fair and deterministic arbitration without relying on any standard library priority queue.

**Interruptible sleep (`smart_sleep`)**
Coders do not use a plain `usleep` for compile/debug/refactor durations. Instead, a loop checks `end_simulation` every millisecond, allowing threads to react quickly when the monitor stops the simulation.

**Race condition prevention example**
Reading `end_simulation` is always done under its mutex lock, even in the hot loop of `smart_sleep`. Without this lock, a thread could read a stale cached value and keep running after the simulation has ended, causing spurious output or missed cleanup.

## Resources

- [POSIX Threads Programming — Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- [Coffman conditions — Wikipedia](https://en.wikipedia.org/wiki/Deadlock#Necessary_conditions)
- [`pthread_cond_wait` man page](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html)
- [`gettimeofday` man page](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)

**AI usage**
Claude (claude.ai) was used during this project for the following tasks:
- Clarifying concepts around `pthread_cond_wait`, `pthread_cond_broadcast`, and condition variable spurious wakeups.
- Helping understand how ThreadSanitizer and Helgrind report data races and lock-order violations.
- README

No code was copied directly from AI output.

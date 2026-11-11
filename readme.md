# mybench

A lightweight, header-only benchmarking toolkit for modern C++ (C++20).

`mybench` measures performance at two scales:

- **Code microbenchmarks** — how fast a single function or snippet runs, down to the nanosecond.
- **Command benchmarks** — how long an external program or script takes to run, end to end.

It is built to avoid the classic pitfalls of performance measurement: dead-code
elimination by the compiler, clock resolution, warm-up effects, and measurement noise.

## Table of contents

- [Features](#features)
- [Requirements](#requirements)
- [Build](#build)
- [Usage](#usage)
- [How it works](#how-it-works)
- [Project structure](#project-structure)
- [Roadmap](#roadmap)
- [License](#license)

## Features

**Microbenchmark** (`bench()`):

- Header-only — just include `mybench/bench.hpp`, nothing to link.
- Statistics over many samples: min, median, mean, max, standard deviation.
- Throughput in operations per second.
- Automatic iteration calibration — adapts to the speed of the code under test.
- Warm-up phase — accounts for cold caches and CPU frequency scaling.
- `DoNotOptimize()` — a compiler barrier that prevents dead-code elimination.
- Aligned table output and CSV export.

**Command benchmark** (`benchcmd`):

- Benchmarks any executable, shell command or script.
- Automatically compiles `.c` / `.cpp` source files before running them.
- Suppresses the program's output for a clean report.
- Same statistics (min, median, mean, max, stddev), in milliseconds.

## Requirements

- A C++20 compiler (GCC or Clang — `DoNotOptimize` uses GCC/Clang inline assembly).
- CMake >= 3.16.
- A POSIX system (the command benchmark uses `/dev/null` redirection).

## Build

```bash
cmake -S . -B build
cmake --build build
```

This builds the example programs into `build/`.

## Usage

### Microbenchmarking code

```cpp
#include <mybench/bench.hpp>
#include <vector>

int main() {
    std::vector<mybench::Result> results;

    results.push_back(mybench::bench("vector sum", [] {
        // code to measure
    }));

    mybench::print_table(results);
    mybench::write_csv(results, "results.csv");
    return 0;
}
```

Call `mybench::DoNotOptimize(value)` inside the lambda to stop the compiler from
removing a computation whose result would otherwise be unused.

Run the bundled demo:

```bash
./build/demonstration
```

### Benchmarking a command or script

```bash
# An executable or shell command
./build/benchcmd "sleep 0.05"
./build/benchcmd "ls -R /usr"

# A C source file — compiled automatically, then benchmarked
./build/benchcmd a.c

# Custom number of runs (default: 20)
./build/benchcmd "sleep 0.05" 50
```

Example output:

```
Benchmark : /tmp/mybench_tmp_bin  (20 runs)
   min    2.742 ms
   median 4.261 ms
   mean   4.682 ms
   max    8.736 ms
   stddev 1.872 ms
```

## How it works

- **Batches and samples** — each measurement runs the code in a batch (inner loop) to
  rise above the clock's resolution, repeated over many samples (outer loop) to build a
  distribution.
- **Why several statistics** — the *min* is the least-disturbed run, closest to the pure
  cost of the code; the *median* is robust to outliers; the *standard deviation* tells
  you how much to trust the numbers.
- **`DoNotOptimize`** — inserts an empty inline-assembly instruction that pretends to
  read a value, forcing the compiler to actually compute it, at zero runtime cost.
- **Code vs command** — the microbenchmark isolates the cost of a function by calling it
  in a loop within the same process; the command benchmark measures a whole program,
  process startup included, which is inherently noisier.

## Project structure

```
.
├── CMakeLists.txt
├── include/mybench/
│   ├── bench.hpp       # code microbenchmark: stats, table, CSV
│   └── command.hpp     # external command / source-file benchmark
└── examples/
    ├── demo.cpp
    ├── demonstration.cpp
    └── benchcmd.cpp
```

## Roadmap

Done:

- [x] RAII timer and nanosecond measurement
- [x] Repetition, warm-up, automatic calibration
- [x] Full statistics (min, median, mean, max, stddev) + throughput
- [x] `DoNotOptimize` compiler barrier
- [x] Aligned table + CSV export
- [x] Command / `.c` source benchmarking with clean output

## License

MIT — see license

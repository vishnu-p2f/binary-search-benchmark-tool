# Binary Search Benchmark Tool

This tool benchmarks **Linear Search** vs **Binary Search** on large, sorted datasets. It is designed for beginners exploring the performance differences between `O(n)` and `O(log n)` search algorithms.

---

## Features

- Generates sorted vector of N random integers.
- Runs M random search trials (targets may or may not exist).
- Measures hit rate, min/avg/max search time (in microseconds).
- Calculates speed-up factor of Binary Search vs Linear Search.

---

## Build Instructions

Requires a C++17-compatible compiler.

```bash
# g++ -std=c++17 -O2 benchmark.cpp -o benchmark
g++ -std=c++17 -I./include src/benchmark.cpp -o benchmark
g++ src/benchmark.cpp -o benchmark
```

## Run Instructions

```bash 
./benchmark --size N --trials M
```

## Sample results
```
Results for the given data 
-------------------------
                              Linear                        Binary                        
Hit rate (%)        0.0                           0.0                           
Min time (µs)      -1   (element : 0)               -1   (element : 0)               
Max time (µs)      0    (element : 0)               0    (element : 0)               
Avg time (µs)      63.6                          10.0                          

=== Average Time Comparison ===
Linear Search |  (63.6 µs)
Binary Search |  (10.0 µs)

Binary Search is 6.0x faster than Linear Search for this data input.

```
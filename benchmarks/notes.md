## Benchmarking addOrder()

I added a benchmark executable separate from the main demo program.

Key learnings:
- Benchmarks should avoid printing inside the measured loop.
- I pre-generated orders before starting the timer to avoid measuring random number generation.
- I measured total elapsed time, average latency per order, and throughput.
- `std::chrono::high_resolution_clock` gives a simple starting point for timing C++ code.
- This is not a perfect benchmark yet because it does not measure p50/p95/p99 latency or isolate CPU/cache effects.
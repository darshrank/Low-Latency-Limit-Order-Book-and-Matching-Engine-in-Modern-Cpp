## Benchmarking addOrder()

I added a benchmark executable separate from the main demo program.

Key learnings:
- Benchmarks should avoid printing inside the measured loop.
- I pre-generated orders before starting the timer to avoid measuring random number generation.
- I measured total elapsed time, average latency per order, and throughput.
- `std::chrono::high_resolution_clock` gives a simple starting point for timing C++ code.
- This is not a perfect benchmark yet because it does not measure p50/p95/p99 latency or isolate CPU/cache effects.

## Debug vs Release Benchmark

Initial Debug build:
- Average latency: ~1405 ns/order
- Throughput: ~0.71M orders/sec

Release build:
- Average latency: ~154 ns/order
- Throughput: ~6.5M orders/sec

Learning:
- C++ performance benchmarks must be run in Release mode.
- Debug builds include less optimization and often extra overhead.
- Compiler optimizations such as inlining and loop optimization can significantly change performance.
# Low-Latency Limit Order Book & Matching Engine (C++)

> Built as part of a transition from competitive programming C++ to systems-level C++ with focus on performance and low-latency design.

A simplified single-threaded limit order book and matching engine implemented in modern C++ (C++20), designed to explore low-latency system design, data structure tradeoffs, and performance benchmarking.

---

## Overview

This project simulates a financial exchange matching engine that processes buy and sell orders using price-time priority.

It supports:
- Limit order matching
- Partial and full fills
- Order cancellation
- Real-time order book maintenance
- Performance benchmarking (latency + throughput)

The goal of this project is to transition from competitive programming-style C++ to systems-level C++ with focus on performance, memory, and design.

---

## Features

- Price-time priority matching
- Buy (bid) and sell (ask) order books
- Partial and full order execution
- Order cancellation in O(1)
- Unit tests for correctness
- Benchmarking with:
  - Average latency
  - p50 / p95 / p99 latency
  - Throughput (orders/sec)

---

## Order Matching Logic

Matching follows standard exchange rules:

- Buy order matches lowest available sell price
- Sell order matches highest available buy price
- Matching condition:
  - Buy: `buy_price >= best_sell_price`
  - Sell: `sell_price <= best_buy_price`
- Orders are matched using FIFO at each price level (price-time priority)
- Partial fills are supported

---

## Data Structures

| Component | Structure | Reason |
|----------|----------|--------|
| Buy Book | `std::map<price, std::list<Order>, std::greater<>>` | Highest price priority |
| Sell Book | `std::map<price, std::list<Order>>` | Lowest price priority |
| Order Queue | `std::list<Order>` | Stable iterators for O(1) deletion |
| Order Index | `std::unordered_map<id, OrderLocation>` | O(1) lookup for cancellation |

### Key Insight

Cancellation requires knowing the **exact location** of an order:

```
id → (side, price, iterator)
```

This mapping allows direct access to the order inside the data structure, enabling:

- O(1) lookup
- O(1) deletion

Without this, cancellation would require scanning the price level (O(n)), which is inefficient for low-latency systems.

---

## Project Structure

```
Low-Latency-Limit-Order-Book-and-Matching-Engine-in-Modern-Cpp/
├── include/
│   ├── Order.hpp
│   ├── Trade.hpp
│   └── OrderBook.hpp
├── src/
│   ├── main.cpp
│   └── OrderBook.cpp
├── tests/
│   └── OrderBookTests.cpp
├── benchmarks/
│   └── Benchmark.cpp
├── CMakeLists.txt
├── README.md
└── .gitignore
```

---

## Clone the Repository

```bash
git clone https://github.com/darshrank/Low-Latency-Limit-Order-Book-and-Matching-Engine-in-Modern-Cpp.git
cd Low-Latency-Limit-Order-Book-and-Matching-Engine-in-Modern-Cpp
```

---

## Build Instructions

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

---

## Running

### Run demo

```bash
./orderbook
```

### Run unit tests

```bash
./orderbook_tests
```

### Run benchmarks

```bash
./orderbook_benchmark
```

---

## Notes

- Use **Release builds** for benchmarking (`-O2 / -O3`)
- Debug builds significantly degrade performance
- Running inside WSL may introduce slight overhead compared to native Linux

---

## License

MIT License

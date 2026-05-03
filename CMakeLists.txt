cmake_minimum_required(VERSION 3.20)

project(CppLimitOrderBook VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(orderbook
    src/main.cpp
    src/OrderBook.cpp
)

target_include_directories(orderbook
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(orderbook_benchmark
    benchmarks/Benchmark.cpp
    src/OrderBook.cpp
)

target_include_directories(orderbook_benchmark
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(orderbook_tests
    tests/OrderBookTests.cpp
    src/OrderBook.cpp
)

target_include_directories(orderbook_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
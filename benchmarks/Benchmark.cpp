#include "OrderBook.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

namespace {

double percentile(std::vector<std::int64_t>& values, double p) {
    if (values.empty()) {
        return 0.0;
    }

    std::sort(values.begin(), values.end());

    const auto index = static_cast<std::size_t>((p / 100.0) * (values.size() - 1));
    return static_cast<double>(values[index]);
}

}

int main() {
    constexpr std::uint32_t numberOfOrders = 100'000;

    OrderBook book;

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> sideDist(0, 1);
    std::uniform_int_distribution<int> priceDist(95, 105);
    std::uniform_int_distribution<int> quantityDist(1, 100);

    std::vector<Order> orders;
    orders.reserve(numberOfOrders);

    for(std::uint32_t i = 0; i < numberOfOrders; ++i) {
        orders.push_back(Order {
            .id = i + 1,
            .side = sideDist(rng) == 0 ? Side::Buy : Side::Sell,
            .type = OrderType::Limit,
            .price = priceDist(rng),
            .quantity = static_cast<std::uint32_t>(quantityDist(rng)),
            .timestamp = i + 1
        });
    }

    std::vector<std::int64_t> latenciesNs;
    latenciesNs.reserve(numberOfOrders);

    std::uint64_t totalTrades = 0;

    const auto benchmarkStart = std::chrono::high_resolution_clock::now();

    for (auto& order : orders) {
        const auto start = std::chrono::high_resolution_clock::now();

        auto trades = book.addOrder(order);
        
        const auto end = std::chrono::high_resolution_clock::now();

        const auto latencyNs = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        latenciesNs.push_back(latencyNs);
        totalTrades += trades.size();
    }

    const auto benchmarkEnd = std::chrono::high_resolution_clock::now();

    const auto totalElapsedNs = std::chrono::duration_cast<std::chrono::nanoseconds>(benchmarkEnd - benchmarkStart).count();

    const double averageLatencyNs = static_cast<double>(totalElapsedNs) / numberOfOrders;

    const double throughput = static_cast<double>(numberOfOrders) / (static_cast<double>(totalElapsedNs) / 1'000'000'000.0);

    std::cout << "Benchmark results\n";
    std::cout << "Orders processed: " << numberOfOrders << '\n';
    std::cout << "Trades executed: " << totalTrades << '\n';
    std::cout << "Total time: " << totalElapsedNs << " ns\n";
    std::cout << "Average latency: " << averageLatencyNs << " ns/order\n";
    std::cout << "p50 latency: " << percentile(latenciesNs, 50.0) << " ns\n";
    std::cout << "p95 latency: " << percentile(latenciesNs, 95.0) << " ns\n";
    std::cout << "p99 latency: " << percentile(latenciesNs, 99.0) << " ns\n";
    std::cout << "Throughput: " << throughput << " orders/sec\n";

    return 0;
}
#include "OrderBook.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

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

    const auto start = std::chrono::high_resolution_clock::now();

    std::uint64_t totalTrades = 0;

    for (auto& order : orders) {
        auto trades = book.addOrder(order);
        totalTrades += trades.size();
    }

    const auto end = std::chrono::high_resolution_clock::now();

    const auto elapsedNs = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    const double averageLatencyNs = static_cast<double>(elapsedNs) / numberOfOrders;

    const double throughput = static_cast<double>(numberOfOrders) / (static_cast<double>(elapsedNs) / 1'000'000'000.0);

    std::cout << "Benchmark results\n";
    std::cout << "Orders processed: " << numberOfOrders << '\n';
    std::cout << "Trades executed: " << totalTrades << '\n';
    std::cout << "Total time: " << elapsedNs << " ns\n"; 
    std::cout << "Average Latency: " << averageLatencyNs << " ns/order\n";
    std::cout << "Throughput: " << throughput << " orders/sec\n";

    return 0;
}
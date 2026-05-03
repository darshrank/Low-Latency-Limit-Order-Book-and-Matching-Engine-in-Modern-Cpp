#pragma once

#include "Order.hpp"
#include "Trade.hpp"

#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <vector>
#include <unordered_map>

class OrderBook {
public:
    std::vector<Trade> addOrder(Order order);
    bool cancelOrder(std::uint64_t orderId);
    void printBook() const;

private:
    using OrderQueue = std::list<Order>;

    struct OrderLocation {
        Side side;
        std::int64_t price;
        OrderQueue::iterator iterator;
    };

    std::map<std::int64_t, OrderQueue, std::greater<>> buyOrders_;
    std::map<std::int64_t, OrderQueue> sellOrders_;
    std::unordered_map<std::uint64_t, OrderLocation> orderIndex_;

    std::vector<Trade> matchBuyOrder(Order& incomingOrder);
    std::vector<Trade> matchSellOrder(Order& incomingOrder);

    void addToBook(const Order& order);
};
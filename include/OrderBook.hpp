#pragma once

#include "Order.hpp"
#include "Trade.hpp"

#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <vector>

class OrderBook {
public:
    std::vector<Trade> addOrder(Order order);
    void printBook() const;

private:
    using OrderQueue = std::list<Order>;

    std::map<std::int64_t, OrderQueue, std::greater<>> buyOrders_;
    std::map<std::int64_t, OrderQueue> sellOrders_;

    std::vector<Trade> matchBuyOrder(Order& incomingOrder);
    std::vector<Trade> matchSellOrder(Order& incomingOrder);

    void addToBook(const Order& order);
};
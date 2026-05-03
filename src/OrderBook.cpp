#include "OrderBook.hpp"

#include <iostream>

std::vector<Trade> OrderBook::addOrder(Order order) {
    std::vector<Trade> trades;

    if(order.side == Side::Buy) {
        trades = matchBuyOrder(order);
    } else {
        trades = matchSellOrder(order);
    }

    if(order.quantity > 0 && order.type == OrderType::Limit) {
        addToBook(order);
    }

    return trades;
}

std::vector<Trade> OrderBook::matchBuyOrder(Order& incomingOrder){
    std::vector<Trade> trades;

    while (incomingOrder.quantity > 0 && !sellOrders_.empty()) {
        auto bestSellIt = sellOrders_.begin();
        auto bestSellPrice = bestSellIt->first;

        if (incomingOrder.price < bestSellPrice) {
            break;
        }

        auto& sellQueue = bestSellIt->second;
        auto& restingSellOrder = sellQueue.front();

        std::uint32_t tradedQuantity = std::min(incomingOrder.quantity, restingSellOrder.quantity);

        trades.push_back(Trade{
            .buyOrderId = incomingOrder.id,
            .sellOrderId = restingSellOrder.id,
            .price = bestSellPrice,
            .quantity = tradedQuantity
    });

        incomingOrder.quantity -= tradedQuantity;
        restingSellOrder.quantity -= tradedQuantity;

        if(restingSellOrder.quantity == 0){
            orderIndex_.erase(restingSellOrder.id);
            sellQueue.pop_front();
        }

        if(sellQueue.empty()) {
            sellOrders_.erase(bestSellIt);
        }
    }

    return trades;
}

std::vector<Trade> OrderBook::matchSellOrder(Order& incomingOrder) {
    std::vector<Trade> trades;

    while (incomingOrder.quantity > 0 && !buyOrders_.empty()) {
        auto bestBuyIt = buyOrders_.begin();
        auto bestBuyPrice = bestBuyIt->first;

        if (incomingOrder.price > bestBuyPrice) {
            break;
        }

        auto& buyQueue = bestBuyIt->second;
        auto& restingBuyOrder = buyQueue.front();

        std::uint32_t tradedQuantity = std::min(incomingOrder.quantity, restingBuyOrder.quantity);

        trades.push_back(Trade{
            .buyOrderId = restingBuyOrder.id,
            .sellOrderId = incomingOrder.id,
            .price = bestBuyPrice,
            .quantity = tradedQuantity
    });

        incomingOrder.quantity -= tradedQuantity;
        restingBuyOrder.quantity -= tradedQuantity;

        if(restingBuyOrder.quantity == 0){
            orderIndex_.erase(restingBuyOrder.id);
            buyQueue.pop_front();
        }

        if(buyQueue.empty()) {
            buyOrders_.erase(bestBuyIt);
        }
    }

    return trades;
}

bool OrderBook::cancelOrder(std::uint64_t orderId) {
    auto indexIt = orderIndex_.find(orderId);

    if(indexIt == orderIndex_.end()){
        return false;
    }

    const auto location  = indexIt->second;
    if(location.side == Side::Buy) {
        auto priceLevelIt = buyOrders_.find(location.price);

        if(priceLevelIt != buyOrders_.end()) {
            priceLevelIt->second.erase(location.iterator);

            if(priceLevelIt->second.empty()) {
                buyOrders_.erase(priceLevelIt);
            }
        }
    } else {
        auto priceLevelIt = sellOrders_.find(location.price);

        if (priceLevelIt != sellOrders_.end()) {
            priceLevelIt->second.erase(location.iterator);

            if (priceLevelIt->second.empty()) {
                sellOrders_.erase(priceLevelIt);
            }
        }
    }

    orderIndex_.erase(indexIt);
    return true;
}

void OrderBook::addToBook(const Order& order) {
    if(order.side == Side::Buy) {
        auto& queue = buyOrders_[order.price];
        queue.push_back(order);

        auto iterator = std::prev(queue.end());

        orderIndex_[order.id] = OrderLocation {
            .side = order.side,
            .price = order.price,
            .iterator = iterator
        };
    } else {
        auto& queue = sellOrders_[order.price];
        queue.push_back(order);

        auto iterator = std::prev(queue.end());

        orderIndex_[order.id] = OrderLocation{
            .side = order.side,
            .price = order.price,
            .iterator = iterator
        };
    }
}

void OrderBook::printBook() const {
    std::cout << "\nBUY BOOK\n";

    for (const auto& [price, orders] : buyOrders_) {
        std::uint32_t totalQuantity = 0;

        for(const auto& order : orders) {
            totalQuantity += order.quantity;
        }
    
        std::cout << "Price: " << price << " Quantity: " << totalQuantity << '\n';
    }

    std::cout << "\nSELL BOOK\n";
    for (const auto& [price, orders] : sellOrders_) {
        std::uint32_t totalQuantity = 0;

        for(const auto& order : orders) {
            totalQuantity += order.quantity;
        }
    
        std::cout << "Price: " << price << " Quantity: " << totalQuantity << '\n';
    }
}
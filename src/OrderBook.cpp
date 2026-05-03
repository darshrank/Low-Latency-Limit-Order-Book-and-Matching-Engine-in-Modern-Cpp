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
            buyQueue.pop_front();
        }

        if(buyQueue.empty()) {
            buyOrders_.erase(bestBuyIt);
        }
    }

    return trades;
}

void OrderBook::addToBook(const Order& order) {
    if(order.side == Side::Buy) {
        buyOrders_[order.price].push_back(order);
    } else {
        sellOrders_[order.price].push_back(order);
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
#include "OrderBook.hpp"

#include <iostream>

void printTrades(const std::vector<Trade>& trades) {
    for (const auto& trade : trades) {
        std::cout << "Trade executed: "
                  << "BuyOrderId=" << trade.buyOrderId
                  << ", SellOrderId=" << trade.sellOrderId
                  << ", Price=" << trade.price
                  << ", Quantity=" << trade.quantity
                  << '\n';
    }
}

int main() {
    OrderBook book;

    book.addOrder(Order{
        .id = 1,
        .side = Side::Buy,
        .type = OrderType::Limit,
        .price = 100,
        .quantity = 10,
        .timestamp = 1
    });

    book.addOrder(Order{
        .id = 2,
        .side = Side::Buy,
        .type = OrderType::Limit,
        .price = 101,
        .quantity = 5,
        .timestamp = 2
    });

    auto trades = book.addOrder(Order{
        .id = 3,
        .side = Side::Sell,
        .type = OrderType::Limit,
        .price = 100,
        .quantity = 12,
        .timestamp = 3
    });

    printTrades(trades);

    book.printBook();

    return 0;
}
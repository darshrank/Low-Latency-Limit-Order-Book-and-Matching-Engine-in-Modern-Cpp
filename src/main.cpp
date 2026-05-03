#include "OrderBook.hpp"

#include <iostream>
#include <vector>

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

void printCancelResult(std::uint64_t orderId, bool cancelled) {
    std::cout << "Cancel order " << orderId << ": "
              << (cancelled ? "success" : "failed")
              << '\n';
}

int main() {
    OrderBook book;

    std::cout << "Adding buy orders...\n";

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

    book.addOrder(Order{
        .id = 3,
        .side = Side::Buy,
        .type = OrderType::Limit,
        .price = 99,
        .quantity = 20,
        .timestamp = 3
    });

    std::cout << "\nInitial book:\n";
    book.printBook();

    std::cout << "\nAdding sell order that partially matches...\n";

    auto trades = book.addOrder(Order{
        .id = 4,
        .side = Side::Sell,
        .type = OrderType::Limit,
        .price = 100,
        .quantity = 8,
        .timestamp = 4
    });

    printTrades(trades);

    std::cout << "\nBook after matching:\n";
    book.printBook();

    std::cout << "\nCancelling resting order 1...\n";
    printCancelResult(1, book.cancelOrder(1));

    std::cout << "\nBook after cancelling order 1:\n";
    book.printBook();

    std::cout << "\nTrying to cancel already-filled order 2...\n";
    printCancelResult(2, book.cancelOrder(2));

    std::cout << "\nTrying to cancel non-existent order 999...\n";
    printCancelResult(999, book.cancelOrder(999));

    std::cout << "\nFinal book:\n";
    book.printBook();

    return 0;
}
#include "Orderbook.hpp"

#include<cassert>
#include<iostream>

void testNoMatchWhenPricesDoNotCross() {
    OrderBook book;

    book.addOrder(Order{1, Side::Buy, OrderType::Limit, 100, 10, 1});

    auto trades = book.addOrder(Order{2, Side::Sell, OrderType::Limit, 101, 5, 2});

    assert(trades.empty());
}

void testFullMatch() {
    OrderBook book;

    book.addOrder(Order{1, Side::Buy, OrderType::Limit, 100, 10, 1});

    auto trades = book.addOrder(Order{2, Side::Sell, OrderType::Limit, 100, 10, 2});

    assert(trades.size() == 1);
    assert(trades[0].buyOrderId == 1);
    assert(trades[0].sellOrderId == 2);
    assert(trades[0].price == 100);
    assert(trades[0].quantity == 10);
}

void testPartialMatch() {
    OrderBook book;

    book.addOrder(Order{1, Side::Buy, OrderType::Limit, 100, 10, 1});

    auto trades = book.addOrder(Order{2, Side::Sell, OrderType::Limit, 100, 4, 2});

    assert(trades.size() == 1);
    assert(trades[0].quantity == 4);

    assert(book.cancelOrder(1));
}

void testCancelExistingOrder() {
    OrderBook book;

    book.addOrder(Order{1, Side::Buy, OrderType::Limit, 100, 10, 1});

    assert(book.cancelOrder(1));
    assert(!book.cancelOrder(1));
}

void testCancelNonExistentOrder() {
    OrderBook book;

    assert(!book.cancelOrder(999));
}

void testBestPricePriority() {
    OrderBook book;

    book.addOrder(Order{1, Side::Buy, OrderType::Limit, 100, 10, 1});
    book.addOrder(Order{2, Side::Buy, OrderType::Limit, 101, 5, 2});

    auto trades = book.addOrder(Order{3, Side::Sell, OrderType::Limit, 100, 5, 3});

    assert(trades.size() == 1);
    assert(trades[0].buyOrderId == 2);
    assert(trades[0].price == 101);
}

int main() {
    testNoMatchWhenPricesDoNotCross();
    testFullMatch();
    testPartialMatch();
    testCancelExistingOrder();
    testCancelNonExistentOrder();
    testBestPricePriority();

    std::cout << "All tests passed.\n";

    return 0;
}
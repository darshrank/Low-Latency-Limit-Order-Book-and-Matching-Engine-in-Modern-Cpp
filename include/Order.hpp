#pragma once

#include<cstdint>
#include<string_view>

enum class Side {
    Buy,
    Sell
};

enum class OrderType {
    Limit,
    Market
};

struct Order {
    std::uint64_t id{};
    Side side{};
    OrderType type{};
    std::int64_t price{};
    std::uint32_t quantity{};
    std::uint64_t timestamp{};
};

constexpr std::string_view toString(Side side) {
    return side == Side::Buy ? "Buy" : "Sell";
}

constexpr std::string_view toString(OrderType type) {
    return type == OrderType::Limit ? "Limit" : "Market";
}
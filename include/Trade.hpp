#pragma once

#include <cstdint>

struct Trade {
    std::uint64_t buyOrderId{};
    std::uint64_t sellOrderId{};
    std::int64_t price{};
    std::uint32_t quantity{};
};
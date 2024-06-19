#pragma  once

#include <cstdint>
#include <limits>

#include "macros.h"

namespace Common {
    constexpr size_t ME_MAX_TICKERS = 8;
    constexpr size_t  ME_MAX_CLIENT_UPDATES = 256 * 1024;
    constexpr size_t  ME_MAX_MARKET_UPDATES = 256 * 1024;
    constexpr size_t  ME_MAX_NUM_UPDATES = 256;
    constexpr size_t  ME_MAX_ORDER_IDS = 1024 * 1024;
    constexpr size_t  ME_MAX_PRICE_LEVELS = 256;

    typedef uint64_t OrderId;
    constexpr auto OrderId_INVALID = std::numeric_limits<OrderId>::max();

    inline auto orderIdToString(OrderID order_id) -> std::string {

    }
}
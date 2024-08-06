#pragma once

#include "common/types.h"
#include "common/mem_pool.h"
#include "common/logging.h"
#include "order_server/client_response.h"
#include "market_data/market_update.h"

#include "me_order.h"

using namespace Common;

namespace Exchange {
    class MatchingEngine;
    class MEOrderBook final {
    public:
        explicit MEOrderBook(TickerId ticker_id, Logger *logger, MatchingEngine *matching_engine);
        ~MEOrderBook();
        auto add(ClientId client_id, OrderId client_order_id, TickerId ticker_id, Side side,
                 Price price, Qty qty) noexcept -> void;
        auto cancel(ClientId client_id, OrderId order_id, TickerId ticker_id) noexcept -> void;
        auto toString(bool detailed, bool validity_check) const -> std::string;

        MEOrderBook() = delete;

        MEOrderBook(const MEOrderBook &) = delete;
        MEOrderBook(const MEOrderBook &&) = delete;

        MEOrderBook &operator=(const MEOrderBook &) = delete;
        MEOrderBook &operator=(const MEOrderBook &&) = delete;

    private:
        TickerId ticker_id_ = TickerId_INVALID;
        MatchingEngine *matching_engine_ = nullptr;

    };
}
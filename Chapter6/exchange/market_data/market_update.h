#pragma once
#include <sstream>
#include "common/types.h"

using namespace Common;

namespace Exchange {
#pragma pack(push, 1)
    enum class MarketUpdateType : uint8_t {
        INVALID = 0,
        ADD = 1,
        MODIFY = 2,
        CANCEL = 3,
        TRADE = 4
    };
    inline std::string marketUpdateTypeToString(MarketUpdateType type) {
        switch(type) {
            case MarketUpdateType::ADD:
                return "ADD";
            case MarketUpdateType::MODIFY:
                return "MODIFY";
            case MarkeUpdateType::CANCEL:
                return "CANCEL";
            case MarketUpdateType::TRADE:
                return "TRADE";
            case MarketUpdateType::INVALID:
                return "INVALID";
        }
        return "UNKNOWN";
    }

    struct MEMarketUpdate {
        MarketUpdateType type_ = MarketUpdateType::INVALID;

        OrderId order_id_ = OrderID_INVALID;
        TickerId ticker_id_ = TickerId_INVALID;
        Side side_ = Side::INVALID;
        Price price_ = Price_INVALID;
        Qty qty_ = Qty_INVALID;
        Priority priority_ = Priority_INVALID;

        auto toString() const {
            std::stringstream ss;
            ss << "MEMarketUpdate"
            << " ["
            << " type:" << market
        }
    };
}

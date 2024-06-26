#pragma once
#include <sstream>
#include "common/lf_queue.h"
#include "common/types.h"

using namespace Common;

namespace Exchange{
#pragma pack(push, 1)
    enum class ClientResponseType : uint8_t {
        INVALID = 0,
        ACCEPTED = 1,
        CANCELED = 2,
        FILLED = 3,
        CANCEL_REJECTED = 4
    };

    inline auto clientResponseTypeToString(ClientResponseType type) -> std::string{
        switch (type) {
            case ClientResponseType::ACCEPTED:
                return "ACCEPTED";
            case ClientResponseType::CANCELED:
                return "CANCELED";
            case ClientResponseType::FILLED:
                return "FILLED";
            case ClientResponseType::CANCEL_REJECTED:
                return "CANCEL_REJECTED";
            case ClientResponseType::INVALID:
                return "INVALID";
        }
        return "UNKNOWN";
    }

    struct MEClientResponse {
        ClientResponseType type_ = ClientResponseType::INVALID;
        ClientId client_id_ = ClientId_INVALID;
        TickerId ticker_id_ = TickerId_INVALID;
        OrderId order_id_ = OrderId_INVALID;
        Side side_ = Side::INVALID;
        Price price_ = Price_INVALID;
        Qty qty_ = Qty_INVALID;


        auto toString() const {
            std::stringstream ss;
            ss << "MEClientResponse"
               << " ["
               << "type:" << clientResponseTypeToString(type_)
               << " client:" << clientIdToString(client_id_)
               << " ticker:" << tickerIdToString(ticker_id_)
               << " oid:" << orderIdToString(order_id_)
               << " side:" << sideToString(side_)
               << " qty:" << qtyToString(qty_)
               << " price:" << priceToString(price_)
               << "]";
            return ss.str();
        }
    };

#pragma pack(pop)
    typedef LFQueue<MEClientResponse> ClientResponseLFQueue;
}

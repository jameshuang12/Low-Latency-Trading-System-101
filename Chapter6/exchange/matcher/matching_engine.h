#pragma once

#include "common/thread_utils.h"
#include "common/lf_queue.h"
#include "common/macros.h"
#include "order_server/client_request.h"
#include "order_server/client_response.h"
#include "market_data/market_update.h"
#include "me_order_book.h"


namespace Exchange{

    class MatchingEngine final{
    public:
        MatchingEngine(ClientRequestLFQueue *client_requests,
            ClientResponseLFQueue *client_responses,
            MEMarketUpdateLFQueue *market_updates);

        ~MatchingEngine();

        auto start() -> void;
        auto stop() -> void;

        MatchingEngine() = delete;
        MatchingEngine(const MatchingEngine &) = delete;
        MatchingEngine(const MatchingEngine &&) = delete;
        MatchingEngine &operator=(const MatchingEngine &) = delete;
        MatchingEngine &operator=(const MatchingEngine &&) = delete;

    private:
        ClientRequestLFQueue *incoming_requests_ = nullptr;
        ClientResponseLFQueue *outgoing_ogw_responses_ = nullptr;
        MEMarketUpdateLFQueue *outgoing_md_updates_ = nullptr;

        volatile bool run_ = false;

    };





}
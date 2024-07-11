#include "me_order_book.h"

#include "matcher/matching_engine.h"

namespace Exchange {
    MEOrderBook::MEOrderBook(TickerId ticker_id, Logger *logger, MatchingEngine *matching_engine)
    : ticker_id_(ticker_id), matching_engine_(matching_engine), order_at_price_pool_(ME_MAX_PRICE_LEVELS),
    order_pool_(ME_MAX_ORDER_IDS), logger_(logger) {

    }

    MEOrderBook::~MEOrderBook() {
        logger_->log("%:% %() % OderBook\n%\n", __FILE__, __LINE__, __FUNCTION__, Common::getCurrentTimeStr(&time_str_),
                     toString(false, true));

        matching_engine_ = nullptr;
        bids_by_price_ = ask_by_price_ = nullptr;
        for (auto &itr: cid_oid_to_order_) {
            itr.fill(nullptr);
        }
    }

}

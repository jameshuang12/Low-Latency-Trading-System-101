#include "me_order_book.h"

#include "matcher/matching_engine.h"

namespace Exchange {
    MEOrderBook::MEOrderBook(TickerId ticker_id, Logger *logger, Exchange::MatchingEngine *matching_engine)
            : ticker_id_(ticker_id), matching_engine_(matching_engine), orders_at_price_pool(ME_MAX_PRICE_LEVELS),
              order_pool_(ME_MAX_ORDER_IDS), logger_(logger){}
}
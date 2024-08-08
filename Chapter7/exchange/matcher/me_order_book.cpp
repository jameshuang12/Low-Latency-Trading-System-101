#include "me_order_book.h"

#include "matcher/matching_engine.h"

namespace Exchange {
    MEOrderBook::MEOrderBook(TickerId ticker_id, Logger *logger, Exchange::MatchingEngine *matching_engine)
            : ticker_id_(ticker_id), matching_engine_(matching_engine), orders_at_price_pool(ME_MAX_PRICE_LEVELS),
              order_pool_(ME_MAX_ORDER_IDS), logger_(logger) {
    }

    MEOrderBook::~MEOrderBook() {
        logger_->log("%:% %() % OrderBook\n%\n", __FILE__, __LINE__, __FUNCTION__, Common::getCurrentTImeStr(&time_str_),
                     toString(false, true));
        matching_engine_ = nullptr;
        bids_by_price = asks_by_price_ = nullptr;
        for (auto &itr: cid_oid_to_order_) {
            itr.fill(nullptr);
        }
    }

    auto MEOrderBook::match(TickerId ticker_id, ClientId client_id, Side side, OrderId client_order_id,
                            OrderId new_market_order_id, MEOrder* itr, Qty* leaves_qty) noexcept {
        const auto order = itr;
        const auto order_qty = order->qty_;
        const auto fill_qty = std::min(*leaves_qty, order_qty);

        *leaves_qty -= fill_qty;
        order->qty_ -= fill_qty;

        client_response_ = {ClientResponseType::FILLED, client_id, ticker_id,
                            client_order_id, new_market_order_id, side, itr->price_,
                            fill_qty, *leaves_qty};
        matching_engine_->sendClientResponse(&client_response_);

        client_response_ = {ClientResponseType::FILLED, order->client_id_, ticker_id,
                            order-client_order_id_, order->market_order_id_, order->side_,
                            itr->price_, fill_qty, order->qty_};
        matching_engine_->sendMarketUpdate(&market_update_);

        market_update_ = {MarketUpdateType::TRADE, OrderId_INVALID, ticker_id, side, itr->price_,
                          fill_qty, Priority_INVALID};
        matching_engine_->sendMarketUpdate(&market_update_);

        if (!order->qty_) {
            market_update_ = {MarketUpdateType::CANCEL, order->market_order_id_, ticker_id,
                              order->side, order->price_, order_qty, Priorty_INVALID};
            matching_engine_->sendMarketUpdate(&market_update_);
            removeOrder(order);
        } else{
            market_update_ = {MarketUpdateType::MODIFY, order->market_order_id, ticker_id, order->side_,
                              order->price_, order->qty_, order->priority_};
            matching_engine_->sendMarketUpdate(&market_update_);
        }
    }


}
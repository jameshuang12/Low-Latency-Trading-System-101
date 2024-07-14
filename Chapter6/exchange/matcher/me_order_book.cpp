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

    auto MEOrderBook::match(TickerId ticker_id, ClientId client_id, Side side, OrderId client_order_id,
                            OrderId new_market_order_id, MEorder* itr, Qty* leaves_qty) noexcept {
        const auto order = itr;
        const auto order_qty = order->qty_;
        const auto fill_qty = std::min(*leaves_qty, order_qty);

        *leaves_qty -= fill_qty;
        order->qty_ -= fill_qty;

        client_response_ = {ClientResponseType::FILLED, client_id, ticker_id,
                            client_order_id, new_market_order_id, side,
                            itr->price_, fill_qty, *leaves_qty};
        matching_engine_->sendClientResponse(&client_response_);

        client_response_ = {ClientResponseType::FILLED, order->client_id, ticker_id, order->client_order_id,
                            order->market_order_id_, order->side_, itr->price_, fill_qty, order->qty_};
        matching_engine_->sendClientResponse(&client_response_);

        market_update_ = {MarketUpdateType::TRADE, OrderId_INVALID, ticker_id, side, itr->price_,
                          fill_qty, Priority_INVALID};
        matching_engine_->sendMarketUpdate(&market_update_);

        if(!order->qty_) {
            market_update_ = {MarketUpdateType::CANCEL, order->market_order_id_,
                              ticker_id, order->side_, order->price_, order_qty, Priority_INVALID};
            matching_engine_->sendMarketUpdate(&market_update_);

            removeOrder(order);
        } else {
            market_update_ = {MarketUpdateType::MODIFY, order->market_order_id_, ticker_id, order->side_,
                              order->price_, order->qty_, order->priority_};
            matching_engine_->sendMarketUpdate(&market_update_);
        }
    }

    auto MEOrder::checkForMatch(ClientID client_id, OrderId client_order_id, TickerId ticker_id,
                                Side side, Price price, Qty qty, Qty new_market_order_id) noexcept {
        auto leaves_qty = qty;

        if(side == Side::BUY) {
            while (leaves_qty && asks_by_price_) {
                const auto ask_itr = asks_by_price_->first_me_order_;
                if (LIKELY(price < ask_itr->price)) {
                    break;
                }
                match(ticker_id, client_id, side, client_order_id_, new_market_order_id, ask_itr, &leaves_qty);
            }
        }
        if (side == Side::SELL) {
            while (leaves_qty && bids_by_price_) {
                const auto bid_itr = bids_by_price_->first_me_order;
                if (LIKELY(price > bid_itr->price_)) {
                    break;
                }
                match(ticker_id, client_id, side, client_order_id, new_market_order_id, bid_itr, &leaves_qty);
            }
        }
        return leaves_qty;
    }

    auto MEOrderBook::add(ClientId client_id, OrderId client_order_id, TickerId ticker_id, Side side,
                          Price price, Qty qty) noexcept -> void {
        const auto new_market_order_id = generateNewMarketOrderId();
        client_response_ = {ClientResponseType::ACCEPTED, client_id, ticker_id,
                            client_order_id, new_market_order_id, side, price, 0 ,qty};
        matching_engine_->sendClientResponse(&client_response_);

        const auto leaves_qty = checkForMatch(client_id, client_order_id, ticker_id, side,
                                              price, qty, new_market_order_id);

        if (LIKELY(leaves_qty)) {
            const auto priority = getNextPriority(price);
            auto order = order_pool_.allocate(ticker_id, client_id, client_order_id, new_market_order_id,
                                              side, price, leaves_qty, priority, nullptr, nullptr);

            addOrder(order);
            market_update_ = {MarketUpdateType::ADD, new_market_order_id, ticker_id, side, price, leaves_qty, priority};
            matching_engine_->sendMarketUpdate(&market_update_);
        }
    }


}

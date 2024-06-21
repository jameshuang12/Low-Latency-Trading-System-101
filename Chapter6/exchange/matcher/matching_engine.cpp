#include "matching_engine.h"


namespace Exchange{

    MatchingEngine::MatchingEngine(Exchange::ClientRequestLFQueue *client_requests,
                                   Exchange::ClientResponseLFQueue *client_responses,
                                   int *market_updates): incoming_requests_(client_requests), outgoing_ogw_responses_(client_responses), outgoing_md_updates_(market_updates)
                               {}


    MatchingEngine::~MatchingEngine(){
        run_ = false;
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);
        incoming_requests_ = nullptr;
        outgoing_md_updates_ = nullptr;
        outgoing_ogw_responses_ = nullptr;
        outgoing_md_updates_ = nullptr;
    }




    auto MatchingEngine::start() -> void {
        run_ = true;
    }

    auto MatchingEngine::stop() -> void {
        run_ = false;
    }

}
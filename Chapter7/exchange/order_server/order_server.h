#pragma once

#include <functional>

#include "common/thread_utils.h"
#include "common/macros.h"
#include "common/tcp_server.h"

#include "order_server/client_request.h"
#include "order_server/client_response.h"
#include "order_server/fifo_sequencer.h"

namespace Exchange {
    class OrderServer {
    public:
        OrderServer(ClientRequestLFQueue *client_requests, ClientResponseLFQueue *client_responses,
                    const std::string &iface, int port);

        ~OrderServer();

        auto start() -> void;
        auto stop() -> void;

    };
}
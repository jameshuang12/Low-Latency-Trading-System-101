#include "time_utils.h"
#include "logging.h"
#include "tcp_server.h"

int main(int, char **) {
    using namespace Common;

    std::string time_str_;

    auto tcpServerRecvCallback = [&](TCPSocket *socket, Nanos rx_time) noexcept {
        logger_.log("TCPServer::defaultREcvCallback() socket:% len:% rx:%\n",
                    socket->socket_fd_, socket->next_rcv_valid_index_, rx_time);

        const std::string reply = "TCPServer received message:" + std::string(socket->inbound_data_.data(),
                                                                              socket->next_rcv_valid_index_);
        socket->next_rcv_valid_index_ = 0;

        socket->send(reply.data(), reply.length());
    };

    auto tcpServerREcvFinishedCallback = [&]() noexcept  {
        logger_.log("TCPServer::defaultRecvFinishedCallback()\n");
    };
}

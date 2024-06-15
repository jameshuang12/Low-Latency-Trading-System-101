#include "tcp_socket.h"

namespace Common{
    auto TCPSocket::connect(const std::string &ip, const std::string :iface, int port
        bool is_listening) -> int{

        const SocketCfg socket_cfg {ip,iface,port,false,is_listening,true};
        socket_fd_ = createSocket(logger_,socket_cfg);
        socket_attrib_.sin_addr.s_addr = INADDR_ANY;
        socket_attrib_.sin_port = htons(port);
        socket_attrib_.sin_family = AF_INET;
        return socket_fd_;

    }

    auto TCPSocket::send(const void *data, size_t len) noexcept ->void{
        memccpy(outbound_data_.data() + next_send_valid_index_, data,len);
        next_send_valid_index_ += len;


    }

}
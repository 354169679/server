#pragma once

#include "Socket.h"

class CliSock : public Socket
{
private:
    // inline void reconnect()
    // {
    //     int fd = get_fd();
    // }

public:
    CliSock() = default;

    CliSock(const std::string &ip_str, const int &port) : Socket(ip_str, port) {}

    // inline void set_ip_and_port(const std::string &ip_str, const int &port)
    // {
    //     init_addr(ip_str, port);
    // }

    inline void connect()
    {
        int fd = get_fd();
        sockaddr_in &addr = get_address();
        int connect_err = ::connect(fd, GENLADDR(addr), sizeof(addr));//connect函数会触发三路握手
        errif(connect_err, "Connection error");
        close(fd);
    }

    ~CliSock()
    {
        int fd = get_fd();
        close(fd);
    }
};

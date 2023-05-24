#pragma once

#include "Socket.h"

class SerSock : public Socket
{
public:
    SerSock(const std::string &ip_str = "127.0.0.1", const int &port = 8899) : Socket(ip_str, port){}

    ~SerSock()
    {
        close(get_fd());
    }

    inline void bind()
    {
        int fd = get_fd();
        sockaddr_in &addr = get_address();
        int bind_err = ::bind(fd, GENLADDR(addr), sizeof(addr));
    }

    inline void listen()
    {
        int fd = get_fd();
        // listen函数在监听到有客户端请求syn报文时，会进行三次握手，假如接收到client第三次握手的ack报文时
        // 就会将成功连接的队列项移到已连接队列
        int listen_err = ::listen(fd, 100);
        errif(listen_err == -1, "listen error");
    }

    inline void accept()
    {
        int fd = get_fd();
        // accept函数在会在三路握手完成的已连接队列中取出客户端进行
        int accept_err = ::accept(fd, nullptr, nullptr);
        errif(accept_err == -1, "accept error");
    }
};

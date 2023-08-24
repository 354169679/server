#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <vector>
#include <string>
#include <iostream>

#include "util.h"

class Socket
{
private:
    int sk_fd_;
    sockaddr_in serv_addr_;

    /// @brief socket函数返回一个初始化socket描述符
    inline void init_socket()
    {
        sk_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        errif(sk_fd_ == -1, "socket create error");
    }



protected:
    /// @brief 初始化ip地址和端口
    /// @param ip
    /// @param port
    inline void init_addr(const std::string &ip, const int &port)
    {
        errif(ip.empty() || port < 0, "init address error");
        bzero(&serv_addr_, sizeof(sockaddr_in));
        serv_addr_.sin_family = AF_INET;
        serv_addr_.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &serv_addr_.sin_addr);
    }

    /// @brief 获取地址结构
    /// @return
    inline sockaddr_in &get_address()
    {
        return serv_addr_;
    }



public:
    Socket() : sk_fd_(-1)
    {
        init_socket();
        bzero(&serv_addr_, sizeof(sockaddr_in));
    }

    Socket(const std::string &ip, const int &port) : sk_fd_(-1)
    {
        init_socket();
        init_addr(ip, port);
    }

    /// @brief 获取socket描述符
    inline int get_fd()const
    {
        return sk_fd_;
    }

    virtual ~Socket() = 0;//使用纯虚析构函数成为抽象类
};

Socket::~Socket(){};

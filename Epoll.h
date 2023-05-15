#pragma once

#include <sys/epoll.h>
#include <unistd.h>


#include <iostream>
#include <unordered_set>

#include "Channel.h"
#include "util.h"



constexpr int MAX_COUNT = 100;

class Epoll
{
private:
    int epoll_fd_;
    epoll_event coming_ev[MAX_COUNT];

public:
    Epoll() : epoll_fd_(-1)
    {
        epoll_fd_ = epoll_create1(0);
        errif(epoll_fd_ == -1, "epoll create error");
    }

    void del_channel(Channel *ch);

    void add_channel(Channel *ch);

    void mod_channel(Channel *ch);

    void poll(Channel::ChannelList &list);

    ~Epoll()
    {
        close(epoll_fd_);
    }
};
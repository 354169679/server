#pragma once

#include <sys/epoll.h>
#include <unistd.h>

#include <iostream>

#include "Channel.h"
#include "util.h"

#include "spdlog/spdlog.h"

constexpr int MAX_COUNT = 100;

class Channel;

class Epoll
{
public:
    using EpollPtr = std::unique_ptr<Epoll>;

private:
    int epoll_fd_;
    epoll_event coming_ev[MAX_COUNT];

private:
    void operate_epoll(Channel::ChannelPtr &ch, int op)
    {
        int channel_fd_ = ch->get_channel_fd();
        epoll_event *ev = new epoll_event;
        ev->data.ptr = ch.get();
        ev->events = ch->get_interest_event();
        int err = epoll_ctl(epoll_fd_, op, channel_fd_, ev);
        errif(err == -1, "epoll delete fd error");
        delete ev;
    }

public:
    Epoll() : epoll_fd_(-1)
    {
        epoll_fd_ = epoll_create1(0);
        errif(epoll_fd_ == -1, "epoll create error");
    }

    void del_channel(Channel::ChannelPtr &ch) // 只能使用引用或者指针，因为调用del_channel隐式使用了拷贝构造函数，ChannelPtr为unique_ptr无法被拷贝
    {
        operate_epoll(ch, EPOLL_CTL_DEL);
    }

    void add_channel(Channel::ChannelPtr &ch)
    {
        operate_epoll(ch, EPOLL_CTL_ADD);
    }

    void mod_channel(Channel::ChannelPtr &ch)
    {
        operate_epoll(ch, EPOLL_CTL_MOD);
    }

    void poll(std::vector<Channel *> &list)
    {
        int err = epoll_wait(epoll_fd_, coming_ev, MAX_COUNT, -1);
        // errif(err == -1, "epoll wait error");
    again:
        if (errno == EAGAIN || errno == EINTR)
        {
            goto again;
        }
        for (int i = 0; i < err; ++i)
        {
            if (coming_ev[i].data.ptr == nullptr)
                continue;
            auto ch_ptr = static_cast<Channel *>(coming_ev[i].data.ptr);
            ch_ptr->set_happend_event(coming_ev[i].events);
            list.push_back(ch_ptr);
        }
    }

    ~Epoll()
    {
        close(epoll_fd_);
    }
};

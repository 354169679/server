#pragma once

#include <sys/epoll.h>
#include <unistd.h>

#include <iostream>
#include <array>

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
    epoll_event ev;
    // epoll_event coming_ev[MAX_COUNT];
    std::array<epoll_event, MAX_COUNT> coming_ev;

private:
    void operate_epoll(const Channel::ChannelPtr &ch, int op)
    {
        bzero(&ev, sizeof(ev));
        int channel_fd_ = ch->get_channel_fd();
        ev.data.ptr = ch.get();
        ev.events = ch->get_interest_event();
        int err = epoll_ctl(epoll_fd_, op, channel_fd_, &ev);
        errif(err == -1, "epoll delete fd error");
    }

public:
    Epoll() : epoll_fd_(epoll_create1(EPOLL_CLOEXEC))
    {
        bzero(&ev, sizeof(ev));
        errif(epoll_fd_ == -1, "epoll create error");
    }

    void del_channel(const Channel::ChannelPtr &ch) // 只能使用引用或者指针，因为调用del_channel隐式使用了拷贝构造函数，ChannelPtr为unique_ptr无法被拷贝
    {
        operate_epoll(ch, EPOLL_CTL_DEL);
    }

    void add_channel(const Channel::ChannelPtr &ch)
    {
        operate_epoll(ch, EPOLL_CTL_ADD);
    }

    void mod_channel(const Channel::ChannelPtr &ch)
    {
        operate_epoll(ch, EPOLL_CTL_MOD);
    }

    void poll(std::vector<Channel *> &list)
    {
    again:
        coming_ev.fill(epoll_event());
        int err = epoll_wait(epoll_fd_, coming_ev.data(), MAX_COUNT, -1);

        if (errno == EAGAIN || errno == EINTR)
        {
            goto again;
        }
        else
        {
            errif(err == -1, "epoll wait error");
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

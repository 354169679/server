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
private:
    int epoll_fd_;
    epoll_event coming_ev[MAX_COUNT];

public:
    Epoll() : epoll_fd_(-1)
    {
        epoll_fd_ = epoll_create1(0);
        errif(epoll_fd_ == -1, "epoll create error");
    }

    void del_channel(Channel::ChannelPtr &ch)
    {
        int channel_fd_ = ch->get_channel_fd();
        int err = epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, channel_fd_, nullptr);
        errif(err == -1, "epoll delete fd error");
    }

    void add_channel(Channel::ChannelPtr &ch)
    {
        int channel_fd_ = ch->get_channel_fd();
        epoll_event *ev = new epoll_event;
        ev->data.ptr = ch.get();
        ev->events = ch->get_interest_event();
        int err = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, channel_fd_, ev);
        errif(err == -1, "epoll delete fd error");
        delete ev;
    }

    void mod_channel(Channel::ChannelPtr &ch)
    {
        int channel_fd_ = ch->get_channel_fd();
        epoll_event *ev = new epoll_event;
        ev->data.ptr = ch.get();
        ev->events = ch->get_interest_event();
        int err = epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, channel_fd_, ev);
        errif(err == -1, "epoll delete fd error");
        delete ev;
    }

    void poll(std::vector<Channel *> &list)
    {
        int err = epoll_wait(epoll_fd_, coming_ev, MAX_COUNT, -1);
        errif(err == -1, "epoll wait error");
again:
        if (errno == EAGAIN)
        {
            goto again;
        }
        for (int i = 0; i < err; ++i)
        {
            if (coming_ev[i].data.ptr == nullptr)
                continue;
            auto ch = static_cast<Channel *>(coming_ev[i].data.ptr);
            ch->set_happend_event(coming_ev[i].events);
            list.push_back(ch);
        }
    }

    ~Epoll()
    {
        close(epoll_fd_);
    }
};

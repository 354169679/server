#pragma once

#include <sys/epoll.h>
#include <unistd.h>
#include <assert.h>
#include <iostream>

#include "Channel.h"

class Epoll : NoCopy
{
private:
    int epoll_fd_;
    epoll_event *events_;

private:
    void FillActionList(std::vector<Channel *> &active_list, int res)
    {
        for (int i = 0; i < res; ++i)
        {
            if (events_[i].data.ptr == nullptr)
                continue;
            Channel *channel = static_cast<Channel *>(events_[i].data.ptr);
            channel->revents_ = events_->events;
            active_list.push_back(channel);
        }
    }

public:
    Epoll() : epoll_fd_(epoll_create1(0)), events_(nullptr)
    {
        assert(epoll_fd_ != -1);
    }

    /// @brief operarte epoll interface
    /// @param ch  channel
    /// @param op  operarte
    void ChangeEpoll(const ChannelPtr &channel, int op)
    {
        epoll_event ev;
        if (op != EPOLL_CTL_DEL)
        {
            ev.data.ptr = channel.get();
            ev.events = channel->events_;
        }

        if (epoll_ctl(epoll_fd_, op, channel->fd_, &ev) < 0)
        {
            throw std::runtime_error(strerror(errno));
        }
    }

    /// @brief epoll_wait wrapper
    /// @param list channel active list
    void Poll(std::vector<Channel *> &active_list, const int channel_list_size)
    {
        events_ = new epoll_event[channel_list_size * 2];
    again:
        int res = epoll_wait(epoll_fd_, events_, channel_list_size, -1);
        if (errno == EINTR && res == -1)
        {
            goto again;
        }
        else if (errno != EINTR && res == -1)
        {
            throw std::runtime_error("epoll wait");
        }
        FillActionList(active_list, res);
        delete events_;
        events_ = nullptr;
    }

    ~Epoll()
    {
        close(epoll_fd_);
        if (events_ != nullptr)
        {
            delete events_;
        }
    }
};

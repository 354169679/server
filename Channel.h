#pragma once

#include <sys/epoll.h>

#include <cassert>
#include <cstring>
#include <iostream>
#include <functional>
#include <memory>
#include <unordered_set>

#include "util.h"
#include "spdlog/spdlog.h"

class EventLoop;

class Channel
{
public:
    using EventCbFun = std::function<void(EventLoop *loop, Channel *ch)>;
    using ChannelPtr = std::unique_ptr<Channel>;
    using ChannelList = std::unordered_set<ChannelPtr>;

private:
    EventLoop *loop_;
    int fd_;
    uint32_t interest_event_;
    uint32_t happend_event_;
    EventCbFun read_cb_;

public:
    Channel(EventLoop *loop = nullptr, int fd = -1, uint32_t interest_event = 0)
        : loop_(loop),
          fd_(fd),
          interest_event_(interest_event),
          happend_event_(0)
    {
        assert(fd != -1 && loop != nullptr);
    }

    inline void set_cb(const EventCbFun &cb)
    {
        read_cb_ = cb;
    }

    inline void set_cb(EventCbFun &&cb)
    {
        read_cb_ = std::move(cb);
    }

    inline int get_channel_fd() const
    {
        return fd_;
    }

    inline void set_interest_event(uint32_t ev)
    {
        interest_event_ = ev;
    }

    inline uint32_t get_interest_event() const
    {
        return interest_event_;
    }

    inline void set_happend_event(uint32_t ev)
    {
        happend_event_ = ev;
    }

    inline uint32_t get_happend_event() const
    {
        return happend_event_;
    }

    inline void handle_event()
    {
        spdlog::info("handle_event");
        if (happend_event_ & EPOLLIN)
        {
            
            read_cb_(loop_,this);
        }
    }

    ~Channel() = default;
};

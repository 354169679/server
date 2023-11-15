#pragma once

#include <cstring>
#include <cassert>
#include <iostream>
#include <functional>
#include <memory>
#include <unordered_map>
#include <sys/epoll.h>

#include "glog/logging.h"
#include "Copy.h"

class EventLoop;
class Channel;

using ChannelPtr = std::unique_ptr<Channel>;
using CallBack = std::function<void(EventLoop *loop, Channel *chan, void *user_date)>;

class Channel : NoCopy
{
    friend class Epoll;
    friend class EventLoop;

private:
    EventLoop *loop_;
    int fd_;
    uint32_t events_;
    CallBack cb_;
    void *ptr_;
    uint32_t revents_;

public:
    Channel(EventLoop *loop = nullptr, int fd = -1, uint32_t event_type = EPOLLIN, CallBack cb = CallBack(), void *pointer = nullptr)
        : loop_(loop), fd_(fd), events_(event_type), cb_(cb), revents_(0), ptr_(pointer)
    {
        assert(fd > 0);
    }

    inline int GetFd() const
    {
        return fd_;
    }

    inline void HandleEvent(uint32_t rev)
    {
        if (rev & EPOLLIN)
        {
            cb_(loop_, this, ptr_);
        }
        else
        {
            LOG(INFO) << "other event";
        }
    }

    inline void ReadChannel(std::string &str)
    {
        char *buffer = (char *)calloc(1024, sizeof(char));
        ::read(fd_, buffer, 1024);
        str.assign(buffer);
        free(buffer);
    }

    void WriteChannel(const std::string &str)
    {
        ::write(fd_, str.c_str(), str.size());
    }

    ~Channel()
    {
        ::close(fd_);
    }
};

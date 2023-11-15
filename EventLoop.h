#pragma once

#include <memory>
#include <sys/timerfd.h>
#include <functional>
#include <vector>

#include "Copy.h"
#include "Epoll.h"
#include "Channel.h"

#include <glog/logging.h>

class EventLoop : NoCopy
{
private:
    std::unique_ptr<Epoll> epoll_ptr_;
    std::unordered_map<int, ChannelPtr> channel_list_;
    std::vector<Channel *> active_list_;
    bool quit_;

public:
    EventLoop() : epoll_ptr_(new Epoll), quit_(false) {}

    /// @brief 添加fd回调任务
    /// @param fd 关注的文件描述符
    /// @param event_type 文件描述符所关注的事件类型
    /// @param cb 文件描述符所绑定的回调函数
    ChannelPtr &CreateChannel(int fd, uint32_t event_type, const CallBack &cb, void *pointer)
    {
        ChannelPtr channel = std::make_unique<Channel>(this, fd, event_type, cb, pointer);
        assert(channel != nullptr);
        epoll_ptr_->ChangeEpoll(channel, EPOLL_CTL_ADD);
        bool emplace_result = channel_list_.emplace(channel->fd_, std::move(channel)).second;
        if (emplace_result == false)
        {
            LOG(ERROR) << "channel list insert a element error";
        }
        return channel_list_[fd];
    }

    // void EmpalceTimerChannel(int timeout, const CallBack &cb, void *pointer)
    // {
    //     if (timeout <= 0)
    //         return;
    //     int timer_fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
    //     assert(timer_fd > 0);
    //     ChannelPtr channel = std::make_unique<Channel>(this, timer_fd, EPOLLIN, cb, pointer);
    //     assert(channel != nullptr);
    //     epoll_ptr_->ChangeEpoll(channel, EPOLL_CTL_ADD);
    //     bool emplace_result = channel_list_.emplace(channel->fd_, std::move(channel)).second;
    //     if (emplace_result == false)
    //     {
    //         LOG(ERROR) << "channel list insert a element error";
    //     }
    //     itimerspec time;
    //     time.it_value.tv_nsec = 0;
    //     time.it_value.tv_sec = timeout;
    //     time.it_interval.tv_nsec = 0;
    //     time.it_interval.tv_sec = timeout;
    //     timerfd_settime(timer_fd, 0, &time, nullptr);
    // }

    /// @brief exit eventloop mode
    inline void Stop()
    {
        quit_ = true;
    }

    /// @brief start eventloop mode
    void Start()
    {
        while (!quit_)
        {
            active_list_.clear();
            epoll_ptr_->Poll(active_list_, channel_list_.size());
            for (auto &channel : active_list_)
            {
                channel->HandleEvent(channel->revents_);
            }
        }
    }

    /// @brief delete a channel
    /// @param fd fd associated with the channel
    inline void EraseChannel(int fd)
    {
        epoll_ptr_->ChangeEpoll(channel_list_.at(fd), EPOLL_CTL_DEL);
        channel_list_.erase(fd);
    }

    ~EventLoop()
    {
        channel_list_.clear();
        active_list_.clear();
    }
};

#pragma once

#include <memory>
#include <functional>
#include <vector>

#include "NoCopy.h"
#include "Epoll.h"

#include "spdlog/spdlog.h"



class EventLoop : NoCopy
{
private:
    Epoll::EpollPtr epoll_ptr;
    Channel::ChannelList list_;
    std::vector<Channel*> active_list_;
    bool quit_;

    inline void add_channel_to_list(Channel::ChannelPtr &&channel_ptr)
    {
        list_.insert(std::move(channel_ptr));
    }

public:
    EventLoop(Epoll *ep = nullptr) : epoll_ptr(std::make_unique<Epoll>()), quit_(false) {}
    ~EventLoop() = default;

    /// @brief 添加fd回调任务
    /// @param fd 关注的文件描述符
    /// @param cb 文件描述符所绑定的回调函数
    /// @param ev 文件描述符所关注的事件类型
    void add_event(int fd, const Channel::EventCbFun &cb, uint32_t ev)
    {
        Channel::ChannelPtr channel_ptr = std::make_unique<Channel>(this, fd, ev);
        errif(channel_ptr == nullptr, "channel ptr is nullptr");
        channel_ptr->set_cb(cb);
        epoll_ptr->add_channel(channel_ptr);
        add_channel_to_list(std::move(channel_ptr));
    }

    void add_timer_event(const std::function<void> &cb)
    {

    }

    void quit()
    {
        quit_ = true;
    }

    void start()
    {
        while (!quit_)
        {
            active_list_.clear();
            epoll_ptr->poll(active_list_);
            spdlog::info("get epoll,sum active list:{}",active_list_.size());
            sleep(1);
            for (auto &i:active_list_)
            {
                i->handle_event();
            }
        }
    }
};

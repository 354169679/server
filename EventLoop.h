#pragma once

#include <memory>
#include <unordered_set>


class Epoll;

class EventLoop
{
private:
    Epoll *ep_;
    std::unordered_set<std::unique_ptr<Channel>> list_;
    bool quit_;

    inline void add_channel_to_list(std::unique_ptr<Channel> &&channel_ptr)
    {
        list_.insert(std::move(channel_ptr));
    }

public:
    EventLoop() = default;
    ~EventLoop() = default;

    EventLoop(Epoll *ep) : ep_(ep), quit_(false) {}

    void add_event(int fd, uint32_t ev, const std::function<void()> &cb)
    {
        std::unique_ptr<Channel> channel_ptr = std::make_unique<Channel>(fd, ev, cb, this);
        if (channel_ptr == nullptr)
            quit();
        add_channel_to_list(std::move(channel_ptr));
    }

    inline void quit()
    {
        quit_ = true;
    }

    // void loop()
    // {
    //     while (!quit_)
    //     {
    //         list_.clear();
    //         ep_->poll(list_);
    //         for (auto &i : list_)
    //         {
    //             i->handle_event();
    //         }
    //     }
    // }
};

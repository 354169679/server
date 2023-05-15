#include "Epoll.h"

void Epoll::del_channel(Channel* ch)
{
    int channel_fd_ = ch->get_channel_fd();
    int err = epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, channel_fd_, nullptr);
    errif(err == -1, "epoll delete fd error");
}

void Epoll::add_channel(Channel *ch)
{
    int channel_fd_ = ch->get_channel_fd();
    epoll_event *ev = new epoll_event;
    ev->data.ptr = ch;
    ev->events = ch->get_interest_event();
    int err = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, channel_fd_, ev);
    errif(err == -1, "epoll delete fd error");
    delete ev;
}

void Epoll::mod_channel(Channel *ch)
{
    int channel_fd_ = ch->get_channel_fd();
    epoll_event *ev = new epoll_event;
    ev->data.ptr = ch;
    ev->events = ch->get_interest_event();
    int err = epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, channel_fd_, ev);
    errif(err == -1, "epoll delete fd error");
    delete ev;
}

void Epoll::poll(Channel::ChannelList &list)
{
    int err = epoll_wait(epoll_fd_, coming_ev, MAX_COUNT, -1);
    std::cout << "epoll wait" << std::endl;
    errif(err == -1, "epoll wait error");
    for (int i = 0; i < err; ++i)
    {
        if (coming_ev[i].data.ptr == nullptr)
            continue;
        // Channel *ch = static_cast<Channel *>(coming_ev[i].data.ptr);
        list.insert(std::begin(coming_ev), std::end(coming_ev));
    }
}

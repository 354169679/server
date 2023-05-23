
#include "SerSock.h"
#include "Channel.h"
#include "EventLoop.h"

#include "spdlog/spdlog.h"

void client_cb(EventLoop *loop, Channel *ch)
{
    char buf[20] = {0};
    // read(ch->get_channel_fd(), buf, sizeof(buf));
    std::cout << "hello client!" << std::endl;
}

void listen_cb(EventLoop *loop, Channel *ch)
{
    spdlog::info("accept a connection require");
    int client_fd = accept4(ch->get_channel_fd(), nullptr, nullptr, O_NONBLOCK);
    loop->add_event(client_fd, client_cb, EPOLLIN);
}

int main()
{
    SerSock server_socket("127.0.0.1", 8899);

    int listen_fd = server_socket.get_fd();

    EventLoop loop;

    server_socket.bind();
    server_socket.listen();

    loop.add_event(listen_fd, listen_cb, EPOLLIN);

    spdlog::info("start loop");
    loop.start();

    return 0;
}

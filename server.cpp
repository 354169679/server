
#include "ServerSock.h"
#include "Channel.h"
#include "EventLoop.h"

#include "spdlog/spdlog.h"

void client_cb(EventLoop *loop, Channel *ch)
{
    char buf[20]={0};
    int fd = ch->get_channel_fd();
    read(fd, buf, sizeof(buf));
    write(fd, buf, strlen(buf));
    std::cout << "write" << std::endl;
}

void listen_cb(EventLoop *loop, Channel *ch)
{
    spdlog::info("accept a connection require");
    int client_fd = accept4(ch->get_channel_fd(), nullptr, nullptr, O_NONBLOCK);
    loop->add_fd_to_eventloop(client_fd, client_cb, EPOLLIN);
}

int main()
{
    ServerSock server_socket("127.0.0.1", 8899);

    int listen_fd = server_socket.get_fd();

    EventLoop loop;

    server_socket.bind();
    server_socket.listen();

    loop.add_fd_to_eventloop(listen_fd, std::move(listen_cb), EPOLLIN);

    spdlog::info("start loop");
    loop.start();

    return 0;
}

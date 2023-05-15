
#include "SerSock.hpp"
#include "Channel.hpp"
#include "EventLoop.hpp"

int main()
{
    SerSock server_socket("127.0.0.1", 8899);

    int listen_fd = server_socket.get_fd();

    EventLoop loop;

    server_socket.bind();
    server_socket.listen();

    server_socket.accept();
    

    return 0;
}
#include "CliSock.h"


int main()
{
    CliSock client_socket("127.0.0.1", 8899);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    client_socket.connect();

    write(client_socket.get_fd(), "xzhyyds", sizeof("xzhyyds"));

    

    return 0;
}

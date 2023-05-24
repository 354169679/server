// #include "CliSock.h"

// int main()
// {
//     CliSock client_socket("127.0.0.1", 8899);

//     sockaddr_in addr;
//     memset(&addr, 0, sizeof(addr));

//     client_socket.connect();

//     int fd = client_socket.get_fd();

//     char buffer[20];
//     write(fd, "xzhyyds", sizeof("xzhyyds"));
//     read(fd, buffer, sizeof(buffer) - 1);

//     printf("Message form server: %s\n", buffer);

//     close(fd);

//     return 0;
// }

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
int main()
{
    // 创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    // 向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));           // 每个字节都用0填充
    serv_addr.sin_family = AF_INET;                     // 使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 具体的IP地址
    serv_addr.sin_port = htons(8899);                   // 端口
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // 读取服务器传回的数据
    char buffer[40];

    while (1)
    {
        bzero(buffer, sizeof(buffer));
        read(STDIN_FILENO, buffer, sizeof(buffer));
        write(sock, buffer, sizeof(buffer));
        bzero(buffer, 40);
        read(sock, buffer, sizeof(buffer) - 1);
        printf("Message form server: %s\n", buffer);
    }

    // 关闭套接字
    close(sock);
    return 0;
}

/********************************************************************************
* @Author : hexne
* @Date   : 2026/05/01 17:49:03
********************************************************************************/

module;
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
export module client;
import log;
import std;
import std.compat;
import net;
import user;

export class Client {
public:

};

export void client_main() {
    sleep(1);

    // 发送到这个地址
    // int fd = socket(AF_INET, SOCK_STREAM, 0);

    Address addr("127.0.0.1", 8080);

    Socket socket(addr);

    // connect(fd, addr.socket_address(), addr.size());
    socket.connect();

    char msg[] = "Hello Echo Server!";
    // Buffer buf;
    socket.send(std::span<char>{msg});
    // send(fd, msg, strlen(msg), 0);

    char buf[1024];
    ssize_t n = socket.recv(std::span<char>{buf});
    if (n > 0) {
        printf("Client received: %.*s\n", (int)n, buf);
    }

}
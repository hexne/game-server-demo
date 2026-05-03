/********************************************************************************
* @Author : hexne
* @Date   : 2026/05/01 17:48:37
********************************************************************************/

module;
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
export module server;
import log;
import net;
import database;
import std;

export class Server {
public:

};


export void server_main() {

    Socket socket(Address {"0.0.0.0", 8080});

    // int server_fd = socket(AF_INET, SOCK_STREAM, 0);


    // bind(server_fd, addr.socket_address(), addr.size());
    socket.bind();
    // listen(server_fd);
    socket.listen();

    printf("Server listening...\n");

    // int client_fd = accept(server_fd, NULL, NULL);
    auto client = socket.accept();

    char buf[1024];
    while (1) {
        // ssize_t n = recv(client_fd, buf, sizeof(buf), 0);
        auto n = client.recv(std::span<char>(buf));
        if (n <= 0) break;
        // send(client_fd, buf, n, 0);
        client.send(std::span<char>(buf));
    }

    // close(client_fd);
    // close(server_fd);
}

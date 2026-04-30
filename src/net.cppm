/********************************************************************************
* @Author : hexne
* @Date   : 2026/04/23 14:30:19
********************************************************************************/
module;
#include <sys/epoll.h>
#include <unistd.h>
export module net;
import std;

class Connect {
    int fd_{};
public:
    struct Address {
        std::string ip;
        int port;
        Address(const std::string_view ip,const int port)
            : ip(ip), port(port) {}
    };
    explicit Connect(const int fd) : fd_(fd) {  }
    Connect() {
        fd_ = epoll_create1(EPOLL_CLOEXEC);
    }
    ssize_t read(void *buf, size_t size) {
        return ::read(fd_, buf, size);
    }
    ssize_t write(void *buf, size_t size) {
        return ::write(fd_, buf, size);
    }

    ~Connect() {
        ::close(fd_);
    }

};

export class Client {
public:
    Client(const std::string_view ip, const int port) {
        Connect::Address addr(ip, port);

    }

};

export class Server {
public:
    Server(const std::string_view ip, const int port) {
        Connect::Address addr(ip, port);
    }

    void listen() {
        

    }

};

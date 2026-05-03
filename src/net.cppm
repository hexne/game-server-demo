/********************************************************************************
* @Author : hexne
* @Date   : 2026/04/23 14:30:19
********************************************************************************/
module;
#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/epoll.h>
export module net;
import std;

template <size_t N = 1024>
class Buffer {
    std::array<char, N> buf_{};
public:
    auto span() {
        return std::span { buf_.data(), buf_.size() };
    }
    auto std::span() {
        return span();
    }
};

export class Address {
    sockaddr_in addr_{};
public:
    // 默认构造：0.0.0.0:0
    Address() {
        addr_.sin_family = AF_INET;
        addr_.sin_addr.s_addr = INADDR_ANY;
        addr_.sin_port = 0;
    }

    // 从 IP + 端口构造
    Address(const std::string_view ip, const int port) {
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port);

        if (::inet_pton(AF_INET, ip.data(), &addr_.sin_addr) <= 0) {
            throw std::runtime_error("invalid IPv4 address");
        }
    }

    // 从 sockaddr_in 构造（用于 UDP recvfrom）
    explicit Address(const sockaddr_in& sa) : addr_(sa) {}

    // 返回端口（主机字节序）
    int port() const {
        return ntohs(addr_.sin_port);
    }

    // 返回 IP 字符串
    std::string ip() const {
        char buf[INET_ADDRSTRLEN];
        const char* p = ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
        return p ? std::string(p) : std::string{};
    }

    const sockaddr_in& as_sockaddr_in() const { return addr_; }
    sockaddr_in& as_sockaddr_in() { return addr_; }

    const sockaddr* socket_address() const {
        return reinterpret_cast<const sockaddr*>(&addr_);
    }
    sockaddr* socket_address() {
        return reinterpret_cast<sockaddr*>(&addr_);
    }

    socklen_t size() const { return sizeof(sockaddr_in); }
};


export class Socket {
    int fd_{};
public:
    Socket(int fd) : fd_(fd) {}

    Socket(Socket&& other) noexcept : fd_(other.fd_) {
        other.fd_ = -1;
    }

    Socket& operator=(Socket&& other) noexcept {
        if (this != &other) {
            close();
            fd_ = other.fd_;
            other.fd_ = -1;
        }
        return *this;
    }

    ~Socket() {
        close();
    }

    // ------------------ 基础 ------------------

    int fd() const { return fd_; }

    bool valid() const { return fd_ >= 0; }

    void create(int domain, int type, int protocol) {
        close();
        fd_ = ::socket(domain, type, protocol);
        if (fd_ < 0) {
            throw std::runtime_error("socket() failed");
        }
    }

    void close() {
        if (fd_ >= 0) {
            ::close(fd_);
            fd_ = -1;
        }
    }

    // ------------------ socket 选项 ------------------

    void set_nonblock() {
        int flags = ::fcntl(fd_, F_GETFL, 0);
        if (flags < 0) throw std::runtime_error("fcntl(F_GETFL) failed");

        if (::fcntl(fd_, F_SETFL, flags | O_NONBLOCK) < 0)
            throw std::runtime_error("fcntl(F_SETFL) failed");
    }

    void set_reuseaddr() {
        int yes = 1;
        if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
            throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
    }

    void set_reuseport() {
        int yes = 1;
        if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes)) < 0)
            throw std::runtime_error("setsockopt(SO_REUSEPORT) failed");
    }

    void set_recvbuf(int size) {
        if (::setsockopt(fd_, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) < 0)
            throw std::runtime_error("setsockopt(SO_RCVBUF) failed");
    }

    void set_sendbuf(int size) {
        if (::setsockopt(fd_, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) < 0)
            throw std::runtime_error("setsockopt(SO_SNDBUF) failed");
    }

    // ------------------ 地址绑定 ------------------

    void bind(const Address& addr) {
        auto sa = addr.as_sockaddr_in();
        if (::bind(fd_, reinterpret_cast<const sockaddr*>(&sa), sizeof(sa)) < 0)
            throw std::runtime_error("bind() failed");
    }

    // ------------------ TCP ------------------

    void listen(int backlog) {
        if (::listen(fd_, backlog) < 0)
            throw std::runtime_error("listen() failed");
    }

    int accept() {
        sockaddr_in sa{};
        socklen_t len = sizeof(sa);

        int newfd = ::accept(fd_, reinterpret_cast<sockaddr*>(&sa), &len);
        if (newfd < 0) return -1; // 非阻塞模式下返回 -1 + errno=EAGAIN

        return newfd;
    }

    void connect(const Address& addr) {
        auto sa = addr.as_sockaddr_in();
        if (::connect(fd_, reinterpret_cast<const sockaddr*>(&sa), sizeof(sa)) < 0) {
            if (errno == EINPROGRESS) return; // 非阻塞模式正常
            throw std::runtime_error("connect() failed");
        }
    }

    // ------------------ 通用 IO ------------------

    ssize_t recv(std::span<char> buf) {
        return ::recv(fd_, buf.data(), buf.size(), 0);
    }

    ssize_t send(std::span<char> buf) {
        return ::send(fd_, buf.data(), buf.size(), 0);
    }

    // ------------------ UDP ------------------

    ssize_t recv_from(std::span<char> buf, Address& from) {
        sockaddr_in sa{};
        socklen_t len = sizeof(sa);

        ssize_t n = ::recvfrom(fd_, buf.data(), buf.size(), 0,
                               reinterpret_cast<sockaddr*>(&sa), &len);

        if (n >= 0) {
            from = Address(sa);
        }
        return n;
    }

    ssize_t send_to(std::span<char> buf, const Address& to) {
        auto sa = to.as_sockaddr_in();
        return ::sendto(fd_, buf.data(), buf.size(), 0,
                        reinterpret_cast<const sockaddr*>(&sa), sizeof(sa));
    }
};


export class TCP {
    int fd_ = -1;
    Address local_addr_{};
public:
    TCP() = default;
    explicit TCP(const Address& addr) : local_addr_(addr) {}

    TCP(const TCP&) = delete;
    TCP& operator=(const TCP&) = delete;
    TCP(TCP&&) = default;
    TCP& operator=(TCP&&) = default;

    ~TCP() { close(); }

    bool valid() const { return fd_ >= 0; }
    int raw_fd() const { return fd_; }
    void close() {
        if (fd_ >= 0) { ::close(fd_); fd_ = -1; }
    }

    void create() {
        fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd_ < 0) throw std::runtime_error("socket() failed");
    }

    void bind() {
        auto sa = local_addr_.as_sockaddr_in();
        if (::bind(fd_, (sockaddr*)&sa, sizeof(sa)) < 0)
            throw std::runtime_error("bind() failed");
    }

    void listen(int backlog = 128) {
        if (::listen(fd_, backlog) < 0)
            throw std::runtime_error("listen() failed");
    }

    std::optional<TCP> accept() {
        sockaddr_in sa{};
        socklen_t len = sizeof(sa);
        int newfd = ::accept(fd_, (sockaddr*)&sa, &len);
        if (newfd < 0) return std::nullopt;

        TCP cli;
        cli.fd_ = newfd;
        cli.local_addr_ = Address(sa);
        return cli;
    }

    void connect(const Address& remote) {
        auto sa = remote.as_sockaddr_in();
        if (::connect(fd_, (sockaddr*)&sa, sizeof(sa)) < 0) {
            if (errno == EINPROGRESS) return;
            throw std::runtime_error("connect() failed");
        }
    }

    ssize_t recv(std::span<char> span) {
        return ::recv(fd_, span.data(), span.size(), 0);
    }

    ssize_t send(std::span<char> span) {
        return ::send(fd_, span.data(), span.size(), 0);
    }
};


export class UDP {
    int fd_ = -1;
    Address local_addr_{};
public:
    UDP() = default;
    explicit UDP(const Address& addr) : local_addr_(addr) {}

    UDP(const UDP&) = delete;
    UDP& operator=(const UDP&) = delete;
    UDP(UDP&&) = default;
    UDP& operator=(UDP&&) = default;

    ~UDP() { close(); }

    bool valid() const { return fd_ >= 0; }
    int raw_fd() const { return fd_; }
    void close() {
        if (fd_ >= 0) { ::close(fd_); fd_ = -1; }
    }

    void create() {
        fd_ = ::socket(AF_INET, SOCK_DGRAM, 0);
        if (fd_ < 0) throw std::runtime_error("socket() failed");
    }

    void bind() {
        auto sa = local_addr_.as_sockaddr_in();
        if (::bind(fd_, (sockaddr*)&sa, sizeof(sa)) < 0)
            throw std::runtime_error("bind() failed");
    }

    ssize_t recv_from(std::span<char> span, Address& from) {
        sockaddr_in sa{};
        socklen_t len = sizeof(sa);

        ssize_t n = ::recvfrom(fd_, span.data(), span.size(), 0,
                               (sockaddr*)&sa, &len);
        if (n >= 0) from = Address(sa);
        return n;
    }

    ssize_t send_to(std::span<char> span, const Address& to) {
        auto sa = to.as_sockaddr_in();
        return ::sendto(fd_, span.data(), span.size(), 0,
                        (sockaddr*)&sa, sizeof(sa));
    }
};

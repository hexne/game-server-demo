import std;
import log;
import net;

void Server() {
    Log().push_log("Server start");

    // 服务端绑定到 8080
    Socket server(Address{"0.0.0.0", 8080});
    server.listen();

    // 接受一个客户端连接
    Socket client;
    while (true) {
        auto client_socket = server.accept();
        if (client_socket) {
            client = std::move(client_socket.value());
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    Log().push_log("Server accepted client");

    // 用一个 Socket 包装这个 fd
    // 注意：这里可以扩展 Socket 构造函数支持接收已有 fd

    char buf[1024];
    while (true) {
        auto n = client.read(buf, sizeof(buf));
        if (n <= 0) {
            Log().push_log("Server client closed");
            break;
        }
        buf[n] = '\0';
        Log().push_log(std::string("Server received: ") + buf);

        // 回显给客户端
        client.write(buf, n);
    }
}

void Client() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Log().push_log("Client start");

    Socket client; // 自动分配本地地址
    Address server_addr("127.0.0.1", 8080);
    client.connect(server_addr);

    const char msg[] = "Hello Echo Server!";
    client.write(msg);

    char buf[1024];
    auto n = client.read(buf, sizeof(buf));
    if (n > 0) {
        buf[n] = '\0';
        Log().push_log(std::string("Client received: ") + buf);
    }
}


int main() {
    auto server = std::thread(Server);
    auto client = std::thread(Client);
    server.join();
    client.join();
}

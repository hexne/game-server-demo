import std;
import log;

void Server() {
    Log().push_log("启动Server线程");
    while (true) {
        std::this_thread::yield();
    }
}

void Client() {
    Log().push_log("启动Client线程");
    while (true) {
        std::this_thread::yield();
    }
}


int main() {

    auto server = std::thread(Server);
    auto client = std::thread(Client);

    server.join();
    client.join();

}

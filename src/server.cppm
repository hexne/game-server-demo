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
    Database db("root", "arch", "game");
    Socket socket(Address {"0.0.0.0", 8080});

    socket.bind();
    socket.listen();
    auto client = socket.accept();

    char buf[1024];
    while (true) {
        auto n = client.recv(std::span<char>(buf));
        if (n <= 0)
            break;
        std::string number(buf, n-1);
        std::cout << number << std::endl;
        auto res = db->select("id", "name")
                     .from("users")
                     .where("number = '{}'", number)
                     .exec();

        std::string send_str;
        for (auto user : res) {
            send_str = std::format("{}:{}", user[0], user[1]);
        }

        client.send(std::span<char>(send_str.data(), send_str.size()));
    }
}

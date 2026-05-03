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

bool check_user_password(std::string_view number, std::string_view password) {


}

export void client_main() {
    sleep(1);

    Address addr("127.0.0.1", 8080);

    Socket socket(addr);

    socket.connect();

    socket.send("num10");

    Buffer buf;
    socket.recv(buf.span());
    if (buf.span().size() > 0) {
        std::string ret_info = buf;
        std::cout << ret_info << std::endl;
    }

}
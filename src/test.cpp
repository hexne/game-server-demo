import std;
import client;


void login(std::span<char> arg) {

}

void reg(std::span<char> arg) {

}


int main(int argc, char *argv[]) {
    std::vector<std::unique_ptr<Client>> clients;

    std::map<std::string, std::function<void()>> events;


    std::string line;
    while (std::cin >> line) {
        // split line, get cmd args
        // such as: login(int login_number)
        // func = bind(cmd, args)
        // events(cmd, func)



    }

    return 0;
}

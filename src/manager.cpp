import std;
import client;


std::vector<Client> clients;

void login(std::span<char> arg) {
    std::string tmp(arg.begin(), arg.end());
    std::cout << "login : " << tmp << std::endl;
}

void reg(std::span<char> arg) {

    std::string tmp(arg.begin(), arg.end());
    std::cout << "reg : " << tmp << std::endl;
}


int main(int argc, char *argv[]) {
    std::vector<std::unique_ptr<Client>> clients;

    std::map<std::string, std::function<void(std::span<char>)>> events = {
        { "reg", reg },
        { "login", login }
    };


    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty())
            continue;

        auto pos = line.find(' ');
        auto cmd = line.substr(0, pos);

        if (!events.contains(cmd)) {
            std::cout << "unknown command: " << cmd << std::endl;
            continue;
        }

        auto arg_start = pos == std::string::npos ? line.size() : pos + 1;
        events[cmd](std::span{line.data() + arg_start, line.size() - arg_start});
    }

    return 0;
}

import std;
import client;
import net;

std::vector<std::unique_ptr<Client>> clients;

struct Account {
    std::string name;
    std::string number;
    std::string password;
};

Account get_account(int number) {
    return Account {
        .name = std::format("user{}", number),
        .number = std::format("num{}", number),
        .password = std::format("pass{}", number),
    };
}


void create_client(std::span<char> arg) {
    int number = std::stoi(std::string(arg.data(), arg.size()));
    clients.reserve(number);
    for (int i = 0; i < number; i++) {
        clients.emplace_back(std::make_unique<Client>(Address{"127.0.0.1", 8080}));
    }
    std::cout << "client : " << clients.size() << std::endl;
}

void login(std::span<char> arg) {
    int number = std::stoi(std::string(arg.data(), arg.size()));
    auto count = std::min(static_cast<std::size_t>(number), clients.size());
    std::size_t success = 0;

    for (std::size_t i = 0; i < count; i++) {
        auto account = get_account(static_cast<int>(i + 1));
        if (clients[i]->login(account.number, account.password))
            success++;
    }

    std::cout << "login : " << success << "/" << count << std::endl;
}

void reg(std::span<char> arg) {
    int number = std::stoi(std::string(arg.data(), arg.size()));

    for (std::size_t i = 0; i < number; i++) {
        auto account = get_account(static_cast<int>(i + 1));
        clients[i]->register_user(account.name, account.number, account.password);
    }
}


int main(int argc, char *argv[]) {

    std::map<std::string, std::function<void(std::span<char>)>> events = {
        { "create", create_client },
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

import std;
import client;
import net;
import range;

// std::vector<std::unique_ptr<Client>> clients;
//
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


// void create_client(std::span<char> arg) {
//     int number = std::stoi(std::string(arg.data(), arg.size()));
//     clients.reserve(number);
//     for (int i = 0; i < number; i++) {
//         clients.emplace_back(std::make_unique<Client>(Address{"127.0.0.1", 8080}));
//     }
//     std::cout << "client : " << clients.size() << std::endl;
// }
//
// void login(std::span<char> arg) {
//     int number = std::stoi(std::string(arg.data(), arg.size()));
//     auto count = std::min(static_cast<std::size_t>(number), clients.size());
//     std::size_t success = 0;
//
//     for (std::size_t i = 0; i < count; i++) {
//         auto account = get_account(static_cast<int>(i + 1));
//         if (clients[i]->login(account.number, account.password))
//             success++;
//     }
//
//     std::cout << "login : " << success << "/" << count << std::endl;
// }
//
// void reg(std::span<char> arg) {
//     int number = std::stoi(std::string(arg.data(), arg.size()));
//
//     for (std::size_t i = 0; i < number; i++) {
//         auto account = get_account(static_cast<int>(i + 1));
//         clients[i]->register_user(account.name, account.number, account.password);
//     }
// }
//
struct UserStatus {
    int id{};
    std::string name{};
    std::string status{};
    std::unique_ptr<Client> client;
};
class ClientManager {
    std::unordered_map<int, Account> users_;
    Epoll epoll_;
    bool stop_;
public:
    void epoll_loop() {

        while (stop_) {

        }

    }

    void show() {
        std::println("{:^10}{:^10}{:^20}{:^20}{:^20}", "index", "fd", "id", "name", "status");
        for (int i = 1;i <= users_.size(); ++i) {
            auto &user = users_[i];
            std::println("{:^10}{:^10}{:^20}{:^20}{:^20}", i, "/", "/", user.name, "/");

        }
        // for (auto &[fd, user] : users_) {
            // std::println("{:^10}{:^10}{:^20}{:^20}{:^20}", fd, "/", user.name, "/");
        // }
    }

    void add(int number = 1) {
        static int index = 1;
        for (auto i : Range(number)) {
            users_.insert(std::make_pair(index , get_account(index)));
            index += 1;
        }
    }

    void stop() {
        stop_ = true;
    }

};



void epoll_thread() {

}


int main(int argc, char *argv[]) {
    // std::thread thread(show_user_status);
    ClientManager manager;
    std::jthread thread(&ClientManager::epoll_loop, &manager);

    std::string cmd;

    std::print(":");
    while (std::cin >> cmd) {
        if (cmd == "quit" || cmd == "q" || cmd == "exit") {
            manager.stop();
            thread.request_stop();
            break;
        }
        if (cmd == "h" || cmd == "help") {  }

        else if (cmd == "show") {
            manager.show();
        }
        // add number, 添加number个客户端
        // @TODO，add 的时候就应该创建fd,而不是在login的时候
        else if (cmd == "add") {
            int number;
            std::cin >> number;
            manager.add(number);
        }
        else if (cmd == "up") {
            std::vector<int> ids{};
            std::string id;
            // up 0-2
            if (id.find('-')) {

            }
            // up 0, 2, 5
            else if (id.find(',')) {

            }
            // up id
            else {
                ids.push_back(std::stoi(id));
            }
            // 取id, 分别login
        }

        std::print(":");
    }
    thread.join();

    return 0;
}

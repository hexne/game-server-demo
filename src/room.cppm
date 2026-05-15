/********************************************************************************
* @Author : hexne
* @Date   : 2026/05/15 16:21:39
********************************************************************************/

module;
export module room;
import std;

// 房间
export class Room {
    int room_id{};
    std::vector<int> users_;
public:
    explicit Room(const int id) : room_id(id) {  }
    void add_user(const int id) {
        if (std::ranges::find(users_, id) == users_.end())
            users_.push_back(id);
    }
    void remove_user(const int id) {
        if (const auto it = std::ranges::find(users_, id); it != users_.end()) {
            std::swap(*it, users_.back());
            users_.pop_back();
        }
    }
};
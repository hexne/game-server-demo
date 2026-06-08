/********************************************************************************
* @Author : hexne
* @Date   : 2026/05/04 22:55:06
********************************************************************************/

module;
export module message;
import std;
import net;

using header_type = std::uint32_t;
export namespace header {
    enum class type : header_type {
        // 登录相关
        login,
        login_true,
        login_false,
        register_user,
        logout,

        // 心跳
        heart,

        // 聊天
        chat_send,

        // 房间
        room_create,
        room_create_true,

        // 邀请进入房间
        room_invite,
        room_invite_true,   // 对方同意
        room_invite_false,  // 对方拒绝

        // @TODO 同意房间邀请
        // @TODO 拒绝房间邀请

        room_leave,
        room_info,

        // 匹配
        match_join,
        match_cancel,
        match_success,


        // 系统/错误
        error
    };
    void write(char *buf, type type) {
        auto v = static_cast<header_type>(type);
        std::memcpy(buf, &v, sizeof(v));
    }
    type read(std::span<char> buf) {
        header_type v;
        std::memcpy(&v, buf.data(), sizeof(v));
        return static_cast<type>(v);
    }

    consteval std::size_t header_size() {
        return sizeof (header_type);
    }
}


export namespace message {

    std::size_t write(char *buf, header::type type, std::span<char> msg) {
        std::uint32_t v = static_cast<std::uint32_t>(type);
        std::memcpy(buf, &v, sizeof(v));
        std::memcpy(buf + sizeof(v), msg.data(), msg.size());
        return sizeof(v) + msg.size();
    }

    char *write(char *buf, int number) {
        std::memcpy(buf, &number, sizeof(number));
        return buf + sizeof(number);
    }
    int read(char *buf) {
        int number;
        std::memcpy(&number, buf, sizeof(number));
        return number;
    }
    int read(std::span<char> span) {
        return read(span.data());
    }

}

export using Router = std::map<header::type, std::function<void(std::span<char>, TCP *)>>;

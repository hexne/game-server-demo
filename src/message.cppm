/********************************************************************************
* @Author : hexne
* @Date   : 2026/05/04 22:55:06
********************************************************************************/

module;
export module message;
import std;

/*
 * 000 000 : login
 * 000 001 : heart
 */

export namespace header {
    enum class type : std::uint32_t {
        login, heart
    };
    void write(char *buf, type type) {
        auto v = static_cast<std::uint32_t>(type);
        std::memcpy(buf, &v, sizeof(v));
    }
    type read(char *buf) {
        std::uint32_t v;
        std::memcpy(&v, buf, sizeof(v));
        return static_cast<type>(v);
    }
}


export namespace message {

    std::size_t write(char *buf, header::type type, std::string_view msg) {
        std::uint32_t v = static_cast<std::uint32_t>(type);
        std::memcpy(buf, &v, sizeof(v));
        std::memcpy(buf + sizeof(v), msg.data(), msg.size());
        return sizeof(v) + msg.size();
    }





}
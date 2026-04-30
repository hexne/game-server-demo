/********************************************************************************
* @Author : hexne
* @Date   : 2026/04/30 14:52:57
********************************************************************************/
module;
export module log;
import std;
import lock_free_queue;
import time;

class log {
    std::jthread thread_;
    MPSCQueue<std::string> message_queue_;
    bool queue_empty_{};

    void run() {
        while (!thread_.get_stop_token().stop_requested() || !queue_empty_) {
            auto msg = message_queue_.pop();
            if (!msg) {
                // std::this_thread::yield();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                queue_empty_ = true;
            }
            else {
                std::println("{}", msg.value());
                queue_empty_ = false;
            }
        }
    }
public:
    log() {
        thread_ = std::jthread(&log::run, this);
    }
    void push_log(std::string_view msg, bool info = false, const std::source_location& loc = std::source_location::current()) {

        auto now = LocalTime::now();
        // [time] [file:line] :
        if (info) {
            auto file = std::filesystem::path(loc.file_name()).filename();
            auto message = std::format("[{}] [{}:{}:{}] {}",
                now.get_string(), file, loc.line(), loc.function_name(), msg);
            message_queue_.push(message);
            return;
        }
        auto message = std::format("[{}] {}", now.get_string(), msg);
        message_queue_.push(message);
    }

    ~log() {
        thread_.request_stop();
        thread_.join();
    }
};

export log& Log() {
    static log ret;
    return ret;
}


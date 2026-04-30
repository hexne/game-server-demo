/********************************************************************************
* @Author : hexne
* @Date   : 2026/04/26 11:44:37
********************************************************************************/

module;
export module tools;
import std;

export void message(std::string_view msg) {
    std::println("{}", msg);
}
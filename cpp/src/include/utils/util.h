#ifndef UTIL_H
#define UTIL_H
#include <cxxabi.h>

#include <algorithm>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace util {

const std::string RESET = "\033[0m";
const std::string BLACK = "\033[30m";  /* Black */
const std::string RED = "\033[31m";    /* Red */
const std::string GREEN = "\033[32m";  /* Green */
const std::string YELLOW = "\033[33m"; /* Yellow */
const std::string BLUE = "\033[34m";   /* Blue */
const std::string PURPLE = "\033[35m"; /* Purple */
const std::string CYAN = "\033[36m";   /* Cyan */
const std::string WHITE = "\033[37m";  /* White */

void title(const std::string& title) {
    size_t ascii_len = 0;
    size_t no_ascii_len = 0;
    for (char c : title) {
        if (c >= 0 && c <= 127) {
            ascii_len++;
        } else {
            no_ascii_len++;
        }
    }
    size_t len = ascii_len + no_ascii_len / 3 * 2;
    std::string line;
    for (size_t i = 0; i < len + 2; i++) {
        line += "═";
    }
    std::cout << GREEN;
    std::cout << "╔" + line + "╗" << std::endl;
    std::cout << "║ " + title + " ║" << std::endl;
    std::cout << "╚" + line + "╝" << std::endl;
    std::cout << RESET;
}

void subtitle(const std::string& subtitle) {
    size_t ascii_len = 0;
    size_t no_ascii_len = 0;
    for (char c : subtitle) {
        if (c >= 0 && c <= 127) {
            ascii_len++;
        } else {
            no_ascii_len++;
        }
    }
    size_t len = ascii_len + no_ascii_len / 3 * 2;
    std::string line;
    for (size_t i = 0; i < len + 2; i++) {
        line += "─";
    }
    std::cout << BLUE;
    std::cout << "┌" + line + "┐" << std::endl;
    std::cout << "│ " + subtitle + " │" << std::endl;
    std::cout << "└" + line + "┘" << std::endl;
    std::cout << RESET;
}

void warning(const std::string& warning) {
    size_t ascii_len = 0;
    size_t no_ascii_len = 0;
    for (char c : warning) {
        if (c >= 0 && c <= 127) {
            ascii_len++;
        } else {
            no_ascii_len++;
        }
    }
    size_t len = ascii_len + no_ascii_len / 3 * 2;
    std::string line;
    for (size_t i = 0; i < len + 2; i++) {
        line += "─";
    }
    std::cout << RED;
    std::cout << std::endl;
    std::cout << "┌" + line + "┐" << std::endl;
    std::cout << "│ " + warning + " │" << std::endl;
    std::cout << "└" + line + "┘" << std::endl;
    std::cout << RESET;
}

void suggession(const std::string& suggession) {
    size_t ascii_len = 0;
    size_t no_ascii_len = 0;
    for (char c : suggession) {
        if (c >= 0 && c <= 127) {
            ascii_len++;
        } else {
            no_ascii_len++;
        }
    }
    size_t len = ascii_len + no_ascii_len / 3 * 2;
    std::string line;
    for (size_t i = 0; i < len + 2; i++) {
        line += "─";
    }
    std::cout << PURPLE;
    std::cout << std::endl;
    std::cout << "┌" + line + "┐" << std::endl;
    std::cout << "│ " + suggession + " │" << std::endl;
    std::cout << "└" + line + "┘" << std::endl;
    std::cout << RESET;
}

void note(std::initializer_list<const std::string> notes) {
    std::cout << std::endl;
    std::cout << YELLOW;
    std::string temp{};
    for (auto it = notes.begin(); it != notes.end(); it++) {
        temp.append(*it);
    }
    std::cout << temp << std::endl;
    std::cout << RESET;
}

void note(const std::string& note) {
    std::cout << std::endl;
    std::cout << YELLOW;
    std::cout << note << std::endl;
    std::cout << RESET;
}

void comment(const std::string& comment) {
    std::cout << CYAN;
    std::cout << comment << std::endl;
    std::cout << RESET;
}

std::string get_type_name(const char* name) {
    char* real_name = abi::__cxa_demangle(name, nullptr, nullptr, nullptr);
    std::string result(real_name);
    free(real_name);
    return result;
}

void use_muti_cpp_version() {
    std::string str = "建议:使用多个C++版本去尝试(当前版本: " + std::to_string(__cplusplus) + ")";
    suggession(str);
}

void check_cpp_version(size_t version) {
    if (__cplusplus != version) {
        std::string str = "警告: 建议使用的C++版本: " + std::to_string(version) +
                          " (当前版本: " + std::to_string(__cplusplus) + ")";
        warning(str);
    }
}

} // namespace util
#endif //UTIL_H
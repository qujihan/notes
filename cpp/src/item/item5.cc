#include <memory>

#include "utils/util.h"

void case1() {
    auto func = [](const std::unique_ptr<int>& p) {};
    auto func2 = [](auto& p) {};
}

int main() {
    util::title("优先考虑auto而非显式类型声明");
    case1();
}

#include <vector>

#include "utils/util.h"

auto get_bool_arr() -> std::vector<bool> {
    return {true, false, true, false, true};
}

auto case1() {
    // 此时通过 [] 获取的是一个引用
    // 但是通过 = 后经过复制, 使得
    bool a = get_bool_arr()[3];

    // 此时 b 的类型是代理类型的引用
    // 现在这个写法就悬垂引用了
    auto b = get_bool_arr()[4];

    // 解决方法: 显式类型初始化
    auto c = static_cast<bool>(get_bool_arr()[2]);
}

int main() {
    util::title("auto推导若非己愿, 使用显式类型初始化惯用法");

    util::subtitle("使用std::vector<bool>例子来说明问题");
    case1();
}

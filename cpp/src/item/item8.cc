#include <memory>

#include "utils/util.h"

auto case1() {
    auto a = 0;
    std::cout << "type(a):" << util::get_type_name(typeid(a).name()) << std::endl;
    auto b = NULL;
    std::cout << "type(b):" << util::get_type_name(typeid(b).name()) << std::endl;
    auto c = nullptr;
    std::cout << "type(c):" << util::get_type_name(typeid(c).name()) << std::endl;
}

/*
    case2
 */
template <typename FuncType, typename PtrType>
decltype(auto) func(FuncType f, PtrType p) {
    return f(p);
}

auto f(std::shared_ptr<int> spi) -> int {
    return 5;
}

auto case2() {
    util::note("func(f, nullptr);");
    util::note("会自动推导出正确的类型");
    func(f, nullptr);

    util::note("func(f, 0);");
    util::note("会报错，因为 0 无法推导出正确的类型，而 nullptr 可以");
    util::note("至于为什么会报错, 因为传0后转成int, int无法强转成shared_ptr<int>");
    util::note("即使 shard_ptr<int> a = 0; 可以运行, 但是传参时不行");
    // func(f, 0);
}

int main() {
    util::title("优先考虑 nullptr 而不是 NULL / 0 ");

    util::subtitle("他们的类型不相同");
    case1();

    util::subtitle("nullptr 作为函数模板的实参时，会自动推导出正确的类型，而 0 不会");
    case2();
}

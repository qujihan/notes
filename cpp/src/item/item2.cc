#include <initializer_list>

#include "utils/util.h"

void case1() {
    auto a1 = {1};
    util::note({"auto a = {1} // typeof(a) 是", util::get_type_name(typeid(a1).name())});

    auto a2{1};
    util::note({"auto a{1} // typeof(a) 是", util::get_type_name(typeid(a2).name()),
                "\n这里 effective cpp 写的是 initializer_list, 但是因为编译器实现改了"});

    auto a3(1);
    util::note({"auto a(1) // typeof(a) 是", util::get_type_name(typeid(a3).name())});

    auto a4 = 1;
    util::note({"auto a = 1 // typeof(a) 是", util::get_type_name(typeid(a4).name())});
}

template <typename T>
void func1(T t) {}

template <typename T>
void func2(std::initializer_list<T> t) {}

void case2() {
    // func1({1, 2, 2, 3, 4});
    util::note({"func1({1, 2, 2, 3, 4}); // 编译错误, 因为编译器无法推导出 T 的类型\n",
                "就推导不出来std::initializer_list<T>的T是什么类型"});

    func2({1, 2, 2, 3, 4});
    util::note("func2({1, 2, 2, 3, 4}); // 编译通过, 因为std::initializer_list<T>的T是确定的");
}

int main() {
    util::title("理解 auto 类型推导");

    util::subtitle("auto 对不同类型的推导");
    case1();

    util::subtitle("对于 initializer_list 的推导");
    case2();
}

#include <functional>

#include "utils/util.h"

void show_args(int a, int b, int c) {
    std::cout << "a:" << a << " b: " << b << " c: " << c << std::endl;
}

void usage_1() {
    std::bind(show_args, 1, 2, 3)();

    std::bind(show_args, std::placeholders::_1, 2, 3)(1);

    util::note("顺序最好还是一致, 否则理解起来挺麻烦");
    std::bind(show_args, std::placeholders::_2, std::placeholders::_1, 3)(1, 2);
}

class Test {
public:
    Test() {}
    Test(const Test&) { std::cout << "copy" << std::endl; }
    Test(Test&&) { std::cout << "move" << std::endl; }
    void class_show_args(int a, int b, int c) { std::cout << "a:" << a << " b: " << b << " c: " << c << std::endl; }
};

void usage_2() {
    Test t;
    std::bind(&Test::class_show_args, &t, 1, 2, 3)();
    std::bind(&Test::class_show_args, &t, std::placeholders::_1, 2, 3)(1);
    std::bind(&Test::class_show_args, &t, std::placeholders::_2, std::placeholders::_1, 3)(1, 2);
}

void func1(Test t) {}
void func2(Test& t) {}
void func3(Test&& t) {}
void usage_3() {
    Test t;

    util::note("传入的是左值, 都是拷贝构造");
    auto func_bind1 = std::bind(func1, t);
    auto func_bind2 = std::bind(func2, t);
    auto func_bind3 = std::bind(func3, t);

    util::note("传入的是右值, 都是移动构造");
    auto func_bind_with_move1 = std::bind(func1, std::move(t));
    auto func_bind_with_move2 = std::bind(func2, std::move(t));
    auto func_bind_with_move3 = std::bind(func3, std::move(t));
}

int main() {
    util::subtitle("std::bind 对于普通函数的用法");
    usage_1();

    util::subtitle("std::bind 对于类成员函数的用法");
    usage_2();

    util::subtitle("向bind中传入左值以及右值会发生什么");
    usage_3();
}

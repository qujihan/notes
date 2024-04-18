#include <array>
#include <cstddef>
#include <string>

#include "utils/util.h"

// 聚合类
//
// 在C++11的时候需要满足(存在的问题: 初始化的任务交给了用户)
// - 所有成员都是public
// - 没有定义任何的构造函数(=default不算)
// - 没有类内初始化(C++17取消)
// - 没有基类, 没有vitrual函数
//
// 在C++17的时候有所改进
// 可以基类,但是必须是公有继承,且必须是非虚继承(基类可以不是聚合类)

class A {
public:
    A(int a) { std::cout << "new" << std::endl; }
    A(int a, int b) { std::cout << "new" << std::endl; }
    A(const A& copy_a) { std::cout << "copy" << std::endl; }
    A(const A&& move_a) { std::cout << "move" << std::endl; }
};

void case1() {
    util::note("use A a = 10 (最不推荐的用法)");
    A demo0 = 10;
    util::note("use A a(10)");
    A demo1(10);
    util::note("use A a{10}");
    A demo2{10};
}

void case2() {
    auto func1{[](A a) {}};
    auto func2{[]() -> A { return A(1); }};
    auto func3{[]() -> A { return A{1}; }};
    auto func4{[]() -> A { return {1}; }};

    auto print_to = R"(
        auto func1{[](A a) {}};

        auto func2{[]() -> A {return A(1);}};

        auto func3{[]() -> A {return A{1};}};

        auto func4{[]() -> A {return {1};}};

        所以别构造了, 直接写就行了!
    )";
    util::note(print_to);

    util::note("func1(A(1))");
    func1(A(1));

    util::note("func1(A{1})");
    func1(A{1});

    util::note("func1({1})");
    func1({1});

    util::note("auto demo = func2();");
    auto demo1 = func2();

    util::note("auto demo = func3();");
    auto demo2 = func3();

    util::note("auto demo = func4();");
    auto demo3 = func4();
}

void case3() {
    struct demoClass {
        int a;
        int b;
    };

    util::note("demoClass a[3] = {{1, 2}, {3, 4}, {5, 6}}; // ok");
    demoClass a[3] = {{1, 2}, {3, 4}, {5, 6}};

    util::note("std::array<demoClass, 3> b = {{1, 2}, {3, 4}, {5, 6}}; // not ok");
    // std::array<demoClass, 3> b = {{1, 2}, {3, 4}, {5, 6}};

    util::note("std::array<demoClass, 3> b = {{{1, 2}, {3, 4}, {5, 6}}}; // ok");
    std::array<demoClass, 3> b = {{{1, 2}, {3, 4}, {5, 6}}};
}

int main() {
    util::subtitle("区别1: 可能会多一次拷贝 or 移动");
    case1();
    util::subtitle("区别2: 显示构造的话多一次拷贝");
    case2();
    util::subtitle("注意点: 在使用std::array的时候需要多包一层{}");
    case3();
}

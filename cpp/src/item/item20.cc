#include <memory>

#include "utils/util.h"

class Widget {
public:
    Widget() { std::cout << "Widget()" << std::endl; }
    ~Widget() { std::cout << "~Widget()" << std::endl; }
};
auto case1() {
    // rc = 1
    auto shared_p = std::make_shared<Widget>();
    std::weak_ptr<Widget> weak_p(shared_p);
    // rc = 0, Widget对象被销毁
    shared_p = nullptr;
    weak_p.expired() ? std::cout << "expired" << std::endl : std::cout << "not expired" << std::endl;
}

auto case2() {
    // rc = 1
    auto shared_p = std::make_shared<Widget>();
    std::weak_ptr<Widget> weak_p(shared_p);
    // rc++, rc = 2
    auto shared_p2 = weak_p.lock();
    // rc--, rc = 1
    shared_p = nullptr;
    weak_p.expired() ? std::cout << "expired" << std::endl : std::cout << "not expired" << std::endl;
    std::cout << "count : " << shared_p2.use_count() << std::endl;
}

class B;
class A {
public:
    A() { std::cout << "A()" << std::endl; }
    ~A() { std::cout << "~A()" << std::endl; }
    std::shared_ptr<B> b_ptr;
};
class B {
public:
    B() { std::cout << "B()" << std::endl; }
    ~B() { std::cout << "~B()" << std::endl; }
    std::shared_ptr<A> a_ptr;
};

auto case3() {
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();
    a->b_ptr = b;
    b->a_ptr = a;
}

int main() {
    util::title("当std::shared_ptr可能悬空时使用std::weak_ptr");
    // util::subtitle("case1: ");
    // case1();
    // util::subtitle("case2: ");
    // case2();
    util::subtitle("case3: 循环引用");
    case3();
}

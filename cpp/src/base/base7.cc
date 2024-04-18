#include "utils/util.h"
/*
    Lambda 表达式
    [captures] (params) specifiers exception -> ret { body }

    捕获列表 captures: 
        NOTE: 只能捕获非静态局部变量
        其实可是挺好理解的: 全局变量 or 静态变量 不需要捕获也是可以使用的

        [] (不捕获任何变量), [=] (值捕获), [&] (引用捕获)
        [=, &a], [this](捕获this指针), 
        [*this](捕获this指向的对象的副本, since:c++17)(多线程使用)
        ....

        (since: c++14) 
        1. 可以使用:auto
        2. 可以传右值 (r = std::move(l))

    可选限定符 specifiers: 
        const(default), mutable, inline, constexpr, consteval, constinit

    可选异常说明符 exception: 
        noexcept, throw()

    返回值类型 -> ret: 
        返回值类型, 可以省略(会自动推倒)
        但是初始化列表不行, 需要手动指明
 */

class A {
public:
    void print() { std::cout << "A::print()" << std::endl; }
    void test() {
        auto f = [this] {
            std::cout << "在test()成员函数中, 使用Lambda 表达式中调用类的成员函数: " << std::endl;
            print();
        };

        f();
    }

private:
    int a;
};

int main(){
    int a = 0;
    int b = 1;
    int c = 2;

    auto f1 = [=]() {
        std::cout << "Lambda 初探, 此时f1()调用:"
                  << " a:" << a << " b:" << b << " c:" << c << std::endl;
    };
    std::cout << "在声明f1之后,调用f1之前 a: " << a << std::endl;
    a = 2;
    // 此时f1()中a的值会是0, 因为声明的同时也捕获了a的值
    // 声明以后的值不会影响捕获的值
    std::cout << "中间修改了a的值为" << a << ", f1()调用的时候a的值是多少呢?" << std::endl;
    f1();
    std::cout << std::endl;

    auto f2{[&a]() mutable noexcept { a = 10; }};
    std::cout << "在声明f2之后,调用f2之前 a: " << a << std::endl;
    f2();
    std::cout << "在声明f2之后,在调用f2之后 a: " << a << std::endl;
    std::cout << std::endl;

    auto f3{[=, &a](int d) mutable -> int {
        a = 20;
        b = 4;
        c = 5;
        std::cout << "在f3中:"
                  << " a:" << a << " b:" << b << " c:" << c << std::endl;
        return a + b + c;
    }};
    auto f3_ret = f3(10);
    std::cout << "f3()执行后,此时:"
              << " a:" << a << " b:" << b << " c:" << c << std::endl;
    std::cout << "f3(): a+b+c=" << f3_ret << std::endl;
    std::cout << std::endl;

    // c++14以后可以使用
    auto f4{[](auto a) { return a; }};
    std::cout << "f4(1): " << f4(1) << std::endl;
    std::cout << "f4(\"string_demo\"): " << f4("string_demo") << std::endl;
    std::cout << std::endl;

    // c++14以后可以使用
    std::cout << "广义的捕获, 捕获右值" << std::endl;
    auto int_ptr = std::make_unique<int>(1);
    // int_ptr 不可以拷贝, 只能移动
    auto f5{[v1 = 1, v2 = std::move(int_ptr)](int x, int y) { return x + y + v1 + *v2; }};
    auto f5_ret = f5(2, 3);
    std::cout << "f5(2, 3): " << f5_ret << std::endl;
    std::cout << std::endl;

    A a_obj;
    a_obj.test();
}

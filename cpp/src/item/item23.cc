#include <type_traits>

#include "utils/util.h"

/*
    use for case1
*/
template <typename T>
T&& mymove1(T&& param) {
    return static_cast<T&&>(param);
}

template <typename T>
typename std::remove_reference<T>::type mymove2(T&& param) {
    using ReturnType = typename std::remove_reference<T>::type&&;
    return static_cast<ReturnType>(param);
}

template <typename T>
decltype(auto) mymove3(T&& param) {
    using ReturnType = typename std::remove_reference<T>::type&&;
    return static_cast<ReturnType>(param);
}

void case1() {
    int x = 10;
    util::note("mymove1不能使用的原因是引用折叠");
    // int&& y1 = mymove1(x);
    util::note("使用类型萃取以及using两个结合在一起可以解决这个问题");
    int&& y2 = mymove2(x);
    util::note("使用decltype(auto)使得写法更加简化");
    int&& y3 = mymove3(x);
}

/*
    use for case2
*/
struct A {
    A(int val) : a(val) { std::cout << "create" << std::endl; }
    A(const A& value) {
        std::cout << "copy" << std::endl;
        a = value.a;
    }
    A(A&& value) {
        a = value.a;
        value.a = 0;
        std::cout << "move" << std::endl;
    }
    int a;
};

class Case2Class {
public:
    // explicit Case2Class(A a) : param(std::move(a)) {}
    explicit Case2Class(const A a):param(std::move(a)){}

private:
    A param;
};

void case2() {
    Case2Class testclass{10};
}

int main() {
    util::title("理解 std::move 和 std::forward");

    util::subtitle("自己实现一个std::move会出现的问题");
    case1();

    util::subtitle("构造函数带不带const会影响是拷贝还是移动");
    case2();
    return 0;
}

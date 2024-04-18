#include "utils/util.h"

// typedef 以及 using 在函数指针中的使用
typedef bool (*func1)(int, int); // func1 == bool (*)(int, int)
typedef bool func2(int, int);    // func2 == bool (int, int)

// func4* == func3
using func3 = bool (*)(int, int); //  func3 == bool (*)(int, int)
using func4 = bool(int, int);     //  func4 == bool (int, int)

// 贯穿整个函数指针的例子的一个函数例子, hhh
bool function(int a, int b) {
    return a == b;
}

void function_type() {
    // 1. 初始化
    // f 的类型是 bool (*)(int, int)
    // 如果想取地址, 理论上应该加 &
    // 但是 C++ 允许忽略
    auto f1 = &function;
    auto f2 = function;
    // 这里不使用 auto, 使用 typedef 以及 using 定义的方式
    func1 f3 = &function;
    func3 f4 = function;

    // 2.调用
    // 理论上应该这样调用 (*f)(1, 2), 但是 C++ 允许忽略
    std::cout << (*f1)(1, 2) << std::endl;
    std::cout << f1(1, 2) << std::endl;

    // 3. 函数指针作为参数
    // 理论上只能函数指针作为参数, 但是 C++ 允许忽略
    // 所以 func2 与 func4 也是可以作为参数了...(有点对C++的混乱表示无语)
    bool function_pointer_as_argument1(int a, int b, func1 f);
    bool function_pointer_as_argument2(int a, int b, func2 f);
    bool function_pointer_as_argument3(int a, int b, func3 f);
    bool function_pointer_as_argument4(int a, int b, func4 f);

    // 4. 函数指针作为返回值
    func1 function_pointer_as_return1(int a, int b);
    // func2 不可以做返回值, 因为不是指针
    //
    // error
    // func2 function_pointer_as_return2(int a, int b);
    func3 function_pointer_as_return3(int a, int b);
    // func4 不可以做返回值, 因为不是指针
    //
    // error
    // func4 function_pointer_as_return4(int a, int b);
}

int main() {
    util::subtitle("函数类型");
    function_type();
}
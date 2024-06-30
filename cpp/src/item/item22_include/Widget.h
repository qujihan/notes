
#include <memory>

class Widget {
public:
    Widget();

    // 这里不能 default, 因为 Impl 是一个未完成的类型
    // 一定要在 Impl 完成以后, 才能实现这个析构函数
    // ~Widget() = default;
    ~Widget();

    // 移动构造以及移动赋值与析构函数同理, 不能在这里实现
    // Widget(Widget&& rhs) = default;
    // Widget& operator=(Widget&& rhs) = default;
    Widget(Widget&& rhs);
    Widget& operator=(Widget&& rhs);

private:
    struct Impl;
    /*
        在 unique_ptr 析构的时候, 默认删除器回会使用 delete 来销毁内置于 unique_ptr 中的指针.
        然后在 delete 之前, 通常会使用 C++11 的特性 static_cast 来确保原始指针指向的类型
        不是一个未完成的类型

        未完成的类型: 只声明了, 但是没有定义

        现在 Impl 就是一个未完成的类型
    */
    std::unique_ptr<Impl> pImpl;
};
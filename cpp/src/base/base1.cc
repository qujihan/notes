#include "utils/util.h"
/* 
    本节重点
    <C++ Primer>: 在执行对象拷贝的操作下, 底层const必须一致, 顶层const可以忽略
    这句话是有点绕的, 其实可以简单的理解为: 我在声明中不希望修改的东西, 就别想从我这里去修改这个东西!
*/

void const_pointer() {
    const int* const p = new int(10);
    // 那么写会出错
    // 这个p1一个const都没有, 一定出错嘛!
    // int *p1 = p;

    // 这样就没有问题了
    // 其实还是挺好理解的
    // 1. p 中const的意思是 p 不能被修改, 且 p 指向的值也不能被修改
    // 2. p1 指向的值必须也是不可用修改的就可以了~!
    int const* p1 = p;
}

void const_reference() {
    const int i = 10;
    // 这个错误比较容易发现
    // 声明 i 的时候不希望 i 的值改变
    // 如果这现在这个声明如果可以通过编译
    // 那么 ri 就可以轻松的去修改 i 的值
    // int& ri = i;

    // 这样 ri 就不可以修改 i 的值了
    // 这样就没有问题了
    const int& ri = i;
}

int main() {
    util::subtitle("const 与指针");
    const_pointer();

    util::subtitle("const 与引用");
    const_reference();
}

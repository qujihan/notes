#include "utils/util.h"
struct Test1 {
    int a;
    short b;
    int c;
    short d;
};

struct Test2 {
    int a;
    int b;
    short c;
    short d;
};

struct Test3 {
    int a;
    short b;
};

struct Test4 : public Test3 {
    short c;
};

void align_memory() {
    // 4 + (2+2(对其)) + 4 + (2+2(对其)) = 16
    std::cout << "sizeof(Test1): " << sizeof(Test1) << std::endl; // 16

    // 4 + 2 + 4 + 2 = 12
    std::cout << "sizeof(Test2): " << sizeof(Test2) << std::endl; // 12

    std::cout << "sizeof(Test3): " << sizeof(Test3) << std::endl; // 8

    // 为什么Test4是 12 而不是 8 呢
    // 因为先给 Test3 内存对其以后, 再让 Test4 继承 Test3
    // 所以是 8 + (2 + 2(对其的)) = 12
    // 为什么这么操作呢? 为了强转的时候不会出错
    std::cout << "sizeof(Test4): " << sizeof(Test4) << std::endl; // 12
}

namespace virtual_table {
/**
a1->┌────────┐           ┌────────────┐                       ┌────────────┐           ┌────────┐ <-b1
    │ vtable │ ────┬───> │  type_info │                       │  type_info │ <────┬─── │ vtable │ 
    ├────────┤     │     ├────────────┤                       ├────────────┤      │    ├────────┤ 
    │ data1_ │     │     │ virtual_f1 │──> virtual_fa1(A*) <──│ virtual_f1 │      │    │   A    │ 
    ├────────┤     │     ├────────────┤                       ├────────────┤      │    ├────────┤ 
    │ data2_ │     │     │ virtual_f1 │──> virtual_fa2(A*)  ┌─│ virtual_f1 │      │    │ data3_ │ 
    └────────┘     │     ├────────────┤   virtual_fa2(B*) <─┘ ├────────────┤      │    └────────┘ 
a2->┌────────┐     │     │   ~A_addr  │──> ~A()       ~B() <──│   ~B_addr  │      │    ┌────────┐ <-b2
    │ vtable │ ────┘     └────────────┘                       ├────────────┤      └────│ vtable │ 
    ├────────┤                             virtual_fb1(B*) <──│ virtual_fb1│           ├────────┤ 
    │ data1_ │                                                ├────────────┤           │   A    │ 
    ├────────┤                             virtual_fb2(B*) <──│ vritual_fb2│           ├────────┤ 
    │ data2_ │                                                └────────────┘           │ data3_ │ 
    └────────┘                                                                         └────────┘ 
  */

class A {
public:
    A() {
        data1_ = new int(1);
        data2_ = new int(2);
    }
    virtual void virtual_fa1() { std::cout << "A::virtual_fa1()" << std::endl; }
    virtual void virtual_fa2() { std::cout << "A::virtual_fa2()" << std::endl; }
    void f1() { std::cout << "A::f1()" << std::endl; }
    void f2() { std::cout << "A::f2()" << std::endl; }
    virtual ~A() {
        std::cout << "A::~A()" << std::endl;
        delete data1_;
        delete data2_;
    }

private:
    int* data1_;
    int* data2_;
};

class B : public A {
public:
    B() { data3_ = new int(3); }
    virtual void virtual_fa1() override { std::cout << "B::virtual_fa1()" << std::endl; }

    virtual void virtual_fb1() { std::cout << "B::virtual_fb1()" << std::endl; }
    virtual void virtual_fb2() { std::cout << "B::virtual_fb2()" << std::endl; }
    void f2() { std::cout << "B::f2()" << std::endl; }

    virtual ~B() {
        std::cout << "B::~B()" << std::endl;
        delete data3_;
    }

private:
    int* data3_;
};

using myfunc = void();
void show_vtable() {
    util::subtitle("Show VTable");

    A a;
    // 获取 a 对象的地址
    auto p1 = reinterpret_cast<uintptr_t*>(&a);
    std::cout << "a address: " << p1 << std::endl;
    // 通过 a 对象地址获取虚函数表地址
    auto p2 = reinterpret_cast<uintptr_t*>(*p1);
    std::cout << "vtable address: " << p2 << std::endl;
    // 通过虚函数表地址获取第一个虚函数地址
    auto p3 = reinterpret_cast<myfunc*>(*p2);
    p3();
    // 通过虚函数表地址获取第二个虚函数地址
    auto p4 = reinterpret_cast<myfunc*>(*(p2 + 1));
    p4();
}
}; // namespace virtual_table

int main() {
    util::subtitle("内存对其");
    align_memory();
}
#include "utils/util.h"

/**
a1->┌────────┐    ┌────────────┐                     ┌────────────┐    ┌────────┐<-b1
    │ vtable ├─┐  │  type_info │                     │  type_info │  ┌─┤ vtable │ 
    ├────────┤ ├─>├────────────┤                     ├────────────┤<─┤ ├────────┤ 
    │ data1_ │ │  │ virtual_f1 ├─> virtual_fa1(A*) <─┤ virtual_f1 │  │ │   A    │ 
    ├────────┤ │  ├────────────┤                     ├────────────┤  │ ├────────┤ 
    │ data2_ │ │  │ virtual_f1 ├─> virtual_fa2(A*)  ┌┤ virtual_f1 │  │ │ data3_ │ 
    └────────┘ │  ├────────────┤  virtual_fa2(B*) <─┘├────────────┤  │ └────────┘ 
a2->┌────────┐ │  │   ~A_addr  ├─> ~A()       ~B() <─┤   ~B_addr  │  │ ┌────────┐<-b2
    │ vtable ├─┘  └────────────┘                     ├────────────┤  └─┤ vtable │ 
    ├────────┤                     virtual_fb1(B*) <─┤ virtual_fb1│    ├────────┤ 
    │ data1_ │                                       ├────────────┤    │   A    │ 
    ├────────┤                     virtual_fb2(B*) <─┤ vritual_fb2│    ├────────┤ 
    │ data2_ │                                       └────────────┘    │ data3_ │ 
    └────────┘                                                         └────────┘ 
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
        std::cout << "A::~A(): 否则可能会导致析构不完整(可以将类A析构函数中的virtual删除试试)." << std::endl;
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
        std::cout << "B::~B(): 如果虚类继承, 基类也得是虚函数." << std::endl;
        delete data3_;
    }

private:
    int* data3_;
};

using myfunc = void();
void show_vtable() {
    B b;

    auto p1 = reinterpret_cast<uintptr_t*>(&b);
    std::cout << "b 的地址是: " << p1 << std::endl;
    std::cout << std::endl;

    auto p2 = reinterpret_cast<uintptr_t*>(*p1);
    std::cout << "虚表地址是: " << p2 << std::endl;
    std::cout << std::endl;

    util::note("通过虚表获取 第一个虚函数");
    auto p3 = reinterpret_cast<myfunc*>(*p2);
    p3();
    std::cout << std::endl;

    util::note("通过虚表获取 第二个虚函数");
    auto p4 = reinterpret_cast<myfunc*>(*(p2 + 1));
    p4();
    std::cout << std::endl;

    util::note("通过虚表获取 第四个虚函数\n代码中为什么是 +5? 因为 +4 是析构函数(看注释图)");
    auto p5 = reinterpret_cast<myfunc*>(*(p2 + 5));
    p5();
    std::cout << std::endl;
}

void polymorphism_demo() {
    B b1;
    A* a1 = (A*)malloc(sizeof(B));
    memcpy((void*)a1, (void*)&b1, sizeof(B));
    util::note("使用malloc不会改变vtable");
    a1->virtual_fa1();
    std::cout << std::endl;

    B b2;
    A* a2 = &b2;
    util::note("使用引用不会改变vtable");
    a2->virtual_fa1();
    std::cout << std::endl;

    B b3;
    util::note("使用reinterpret_cast还是不会改变vtable");
    A* a3 = reinterpret_cast<A*>(&b3);
    a3->virtual_fa1();
    std::cout << std::endl;
}

// 运行时多态
void rtti() {
    // 一个函数在传参的时候, 如果是父类的指针/引用
    // 有时候我们需要知道他到底是哪一个子类
    // 继续看我们的图: typeinfo里记录了类的类型!
    B b;
    A* a = &b;

    auto b_addr = reinterpret_cast<uintptr_t*>(&b);
    auto b_vtable_addr = reinterpret_cast<uintptr_t*>(*b_addr);
    std::type_info* b_type_info = reinterpret_cast<std::type_info*>(*(b_vtable_addr - 1));
    std::cout << b_type_info->name() << std::endl;

    // 这是c++库提供的显示类型的方法 typeid()
    //
    // 注意:
    // 1. typeid() 删除了拷贝构造, 只能使用指针或者引用去接收
    // 2. typeid 会忽略 const 修饰
    //
    // 应该与上面是相同的输出
    std::cout << typeid(b).name() << std::endl;
    // 即使打印*a, 也是一样的结果
    std::cout << typeid(*a).name() << std::endl;
}

int main(){
    util::subtitle("展示虚表");
    show_vtable();

    util::subtitle("多态的例子");
    polymorphism_demo();

    util::subtitle("RTTI: 运行时多态");
    rtti();
}
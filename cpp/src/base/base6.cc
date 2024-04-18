#include "utils/util.h"
class vA {
public:
    virtual void f1() { std::cout << "vA::f1()" << std::endl; }
    int a{1};
};

class vB : public vA {
public:
    virtual void f1() override { std::cout << "vB::f1()" << std::endl; }
    int b{2};
};

class A {
public:
    int a{1};
};

class B : public A {
public:
    int b{2};
};

void static_cast_demo() {
    std::cout << "1. 基本类型转换 ..." << std::endl;
    std::cout << std::endl;

    std::cout << "2. 继承√ 多态☓ 值类型转换" << std::endl;
    std::cout << "子转父可以，父转子不可以" << std::endl;
    //
    // Error:no match conversion for static_case from 'A' to 'B'
    // A a1;
    // B b1 = static_cast<B>(a1);
    //
    // 这样就可以了(子转父)
    B b2;
    A a2 = static_cast<A>(b2);
    std::cout << std::endl;

    std::cout << "3. 继承√ 多态√ 值类型转换" << std::endl;
    std::cout << "子转父可以，父转子不可以" << std::endl;
    //
    // Error:no match conversion for static_case from 'vA' to 'vB'
    // vA va1;
    // vB vb1 = static_cast<vB>(va1);
    //
    vB vb2;
    vA va2 = static_cast<vA>(vb2);
    std::cout << std::endl;

    std::cout << "4. 继承√ 多态☓ 指针引用转换" << std::endl;
    std::cout << "都可以，但是不安全(父转子不安全)" << std::endl;
    A* a1_ptr = new A();
    B* b1_ptr = static_cast<B*>(a1_ptr);
    B* b2_ptr = new B();
    A* a2_ptr = static_cast<A*>(b2_ptr);
    std::cout << std::endl;

    std::cout << "5. 继承√ 多态√ 指针引用转换" << std::endl;
    std::cout << "都可以，但是不安全(父转子不安全)" << std::endl;
    vA* va1_ptr = new vA();
    vB* vb1_ptr = static_cast<vB*>(va1_ptr);
    vB* vb2_ptr = new vB();
    vA* va2_ptr = static_cast<vA*>(vb2_ptr);
    std::cout << std::endl;
}

void dynamic_cast_demo() {
    std::cout << "继承√ 多态☓ 指针引用转换" << std::endl;
    std::cout << "没有多态不能使用 dynamic_cast" << std::endl;
    // A* a1_ptr = new A();
    // B* b1_ptr = dynamic_cast<B*>(a1_ptr);
    // B* b2_ptr = new B();
    // A* a2_ptr = dynamic_cast<A*>(b2_ptr);
    std::cout << std::endl;

    std::cout << "继承√ 多态√ 指针引用转换" << std::endl;
    std::cout << "static_cast 不安全, 但是 dynamic_cast 安全" << std::endl;
    vA* va1_ptr = new vA();
    vB* vb1_ptr = dynamic_cast<vB*>(va1_ptr);
    std::cout << "安全体现在父转子如果转不成果会返回nullptr(0x0)" << std::endl;
    std::cout << "vb1_ptr addr:" << vb1_ptr << std::endl;
    vB* vb2_ptr = new vB();
    vA* va2_ptr = dynamic_cast<vA*>(vb2_ptr);
    std::cout << "va2_ptr addr:" << va2_ptr << std::endl;
    std::cout << std::endl;
}

void const_cast_demo() {
    std::cout << "const_cast 去掉底层 const" << std::endl;
    std::cout << "通常用于函数重载" << std::endl;
    std::cout << "实际上基本类型中处理char*不能修改, 其他的都能修改" << std::endl;

    // 这里通过强转的 pa2 去修改 a的值
    int a = 0;
    const int* pa1 = &a;
    int* pa2 = const_cast<int*>(pa1);
    *pa2 = 10;
    std::cout << a << std::endl;
}

void reinterpret_cast_demo() {
    std::cout << "重新解释一段代码" << std::endl;
    A a;
    B* b = reinterpret_cast<B*>(&a);
    std::cout << b->a << std::endl;
    std::cout << "打印b应该是2的, 但是是乱码" << std::endl;
    std::cout << b->b << std::endl;
}

int main(){
    util::subtitle("static cast");
    static_cast_demo();

    util::subtitle("dynamic cast");
    dynamic_cast_demo();

    util::subtitle("const cast");
    const_cast_demo();

    util::subtitle("reinterpret cast");
    reinterpret_cast_demo();
}
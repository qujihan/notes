#include "utils/util.h"

/**
    左值: 可以被取地址
    右值: 不可以被取地址

    所有的字面量都是右值, 除了字符串字面量(是左值)

    左值引用不接受右值, 所以出现了接受右值的右值引用(Type &&)
*/
class BigMemoryPoolWithMove {
public:
    // 4K
    static const int SIZE = 1024 * 4;

    BigMemoryPoolWithMove() : pool_(new char[SIZE]) {
        // std::cout << "new" << std::endl;
    }

    BigMemoryPoolWithMove(const BigMemoryPoolWithMove& other) : pool_(new char[SIZE]) {
        std::cout << "copy" << std::endl;
        memcpy(pool_, other.pool_, SIZE);
    }

    BigMemoryPoolWithMove(BigMemoryPoolWithMove&& other) {
        std::cout << "move" << std::endl;
        pool_ = other.pool_;
        other.pool_ = nullptr;
    }

    ~BigMemoryPoolWithMove() {
        if (pool_ != nullptr) {
            delete[] pool_;
        }
    }

private:
    char* pool_;
};

// 这个 class 与上面的唯一区别是加了一个移动构造函数
class BigMemoryPoolWithoutMove {
public:
    // 4K
    static const int SIZE = 1024 * 4;

    BigMemoryPoolWithoutMove() : pool_(new char[SIZE]) { std::cout << "new" << std::endl; }

    BigMemoryPoolWithoutMove(const BigMemoryPoolWithoutMove& other) : pool_(new char[SIZE]) {
        std::cout << "copy" << std::endl;
        memcpy(pool_, other.pool_, SIZE);
    }

    ~BigMemoryPoolWithoutMove() {
        if (pool_ != nullptr) {
            delete[] pool_;
        }
    }

private:
    char* pool_;
};

// 1.
// 在关闭返回值优化的情况下
// 其实无论是移动构造还是拷贝构造, 都是在栈上创建一个临时对象, 然后把这个临时对象传递给 a
// 换句话说, 都是执行了两遍 构造/拷贝 函数
// 具体执行情况可以看一下 demo1() 的执行过程
//
// 2.
// 在存在移动构造的情况下, 调用 getPool() 时, 会调用移动构造函数, 而不是拷贝构造函数
// 因为知道这个值是个将亡值
BigMemoryPoolWithoutMove getPoolWithoutMove() {
    BigMemoryPoolWithoutMove pool;
    return pool;
}

BigMemoryPoolWithMove getPoolWithMove() {
    BigMemoryPoolWithMove pool;
    return pool;
}

void copy_or_move() {
    // 在关闭返回值优化的情况下
    // set(CMAKE_CXX_FLAGS -fno-elide-constructors)
    //
    // 如果有移动构造就使用移动构造(两次)
    // 如果没有移动构造就使用拷贝构造(两次)
    util::note("如果 不 存在移动构造函数(结果是:copy copy)");
    auto without_move = getPoolWithoutMove();

    util::note("如果存在移动构造函数(结果是:move move)");
    auto with_move = getPoolWithMove();
}

void use_std_move() {
    util::note("with move(结果是:move copy)");
    auto a = BigMemoryPoolWithMove();
    auto b = a;

    util::note("without move(结果是:copy copy)");
    auto c = BigMemoryPoolWithoutMove();
    auto d = c;

    // 可以看到第二个结果都是 copy, 即使存在移动构造也是 copy
    // 这是因为原来的时候知道都是临时变量, 使用编译器是知道move没有问题
    // 现在就需要使用 std::move 来告诉编译器, 这个值是个将亡值
    // 那么就会使用 move 了
    //
    // 但是需要注意: std::move 没有移动, 只是将这个值变成了将亡值
    // 真正移动的操作, 还是在移动构造函数中 BigMemoryPoolWithMove(BigMemoryPoolWithMove&& other)
    util::note("use std::move");
    auto e = std::move(a);
}

// 返回值优化的本质就是把 func1 改成 func2
// 就可以避免一次构造
class RVOTest {
public:
    RVOTest(int i) : data(i) { std::cout << "constructor" << std::endl; }
    ~RVOTest() {
        // std::cout << "destructor" << std::endl;
    }
    RVOTest(const RVOTest& other) { std::cout << "copy" << std::endl; }
    RVOTest(RVOTest&& other) { std::cout << "move" << std::endl; }

private:
    int data{};
};

// func1(为了模拟不具名返回值优化)
RVOTest getRVOTest(int i) {
    return RVOTest(i);
}

// func2(为了模拟不具名返回值优化)
void getRVOTest(RVOTest& test, int i) {
    new (&test) RVOTest(i);
}

void u_return_value_optimization() {
    util::note("使用常规的方式, 会发现有两次移动构造以及两次析构");
    auto without_optimization = getRVOTest(1);

    util::note("使用模拟返回值优化的方式, 就没有了移动构造/拷贝构造");
    char* buff[sizeof(RVOTest)];
    RVOTest* ptr = reinterpret_cast<RVOTest*>(buff);
    getRVOTest(*ptr, 1);
}

// func3
RVOTest getRVOTest2(int i) {
    RVOTest a{i};
    return a;
}

// func4
void getRVOTest2(RVOTest& test, int i) {
    RVOTest a{i};
    // 这里需要使用 std::move 来告诉编译器, 这个值是个将亡值
    new (&test) RVOTest(std::move(a));
}

// func5
void getRVOTest2_2(RVOTest& test, int i) {
    new (&test) RVOTest(i);
}

void n_return_value_optimization() {
    util::note("使用常规的方式, 会发现有两次移动构造以及两次析构");
    auto without_optimization = getRVOTest2(1);

    util::note("使用模拟返回值优化的方式( C++17+ / 且关闭返回值优化的情况下)");
    char* buff[sizeof(RVOTest)];
    RVOTest* ptr = reinterpret_cast<RVOTest*>(buff);
    getRVOTest2(*ptr, 1);

    util::note("使用模拟返回值优化的方式 (开启返回值优化的情况下)");
    char* buff2[sizeof(RVOTest)];
    RVOTest* ptr2 = reinterpret_cast<RVOTest*>(buff2);
    getRVOTest2_2(*ptr2, 1);

    util::note("此处忽略了 C++14 以下且关闭返回值优化的情况下的情况, 因为情况显而易见");
}

int main() {
    util::subtitle("这里是调用拷贝构造还是赋值构造呢?");
    copy_or_move();

    util::subtitle("使用 std::move");
    util::check_cpp_version(201402);
    use_std_move();

    util::subtitle("不具名返回值优化(URVO)");
    util::warning("在C++17后, 不具名返回值优化已经被标准化, 不具名返回值一定会被优化");
    u_return_value_optimization();

    util::subtitle("具名返回值优化(NRVO)");
    util::warning("在C++17后, 具名返回值优化可以通过-fno-elide-constructors参数去修改");
    util::use_muti_cpp_version();
    n_return_value_optimization();
}

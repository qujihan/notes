#include <memory>

#include "utils/util.h"

class Investment {
public:
    virtual ~Investment() {}
};

class Stock : public Investment {
public:
    Stock(int a) { std::cout << "Stock(int a)" << std::endl; }
    ~Stock() override { std::cout << "~Stock()" << std::endl; }
};

class Bond : public Investment {
public:
    Bond(int a, int b) { std::cout << "Bond(int a, int b)" << std::endl; }
    ~Bond() override { std::cout << "~Bond()" << std::endl; }
};

class ReadEstate : public Investment {
public:
    ReadEstate(int a, int b, int c) { std::cout << "ReadEstate(int a, int b, int c)" << std::endl; }
    ~ReadEstate() override { std::cout << "~ReadEstate()" << std::endl; }
};

// 这个在栈上分配内存
// 这里的问题是 栈上的内存不好传出来, 因为在函数结束后就会被释放, ptr 就悬垂引用了
template <typename... T>
Investment* makeInvestment_test1(T&&... params) {
    Investment* ptr;
    constexpr int size = sizeof...(params);
    if constexpr (size == 1) {
        Stock stock{std::forward<T>(params)...};
        ptr = &stock;
    } else if constexpr (size == 2) {
        Bond bond{std::forward<T>(params)...};
        ptr = &bond;
    } else if constexpr (size == 3) {
        ReadEstate readEstate{std::forward<T>(params)...};
        ptr = &readEstate;
    }
    return ptr;
}

// 这个在堆上分配内存
// 这里的问题是, ptr不会悬垂引用, 但是ptr的生命周期不好控制, 需要手动释放
template <typename... T>
Investment* makeInvestment_test2(T&&... params) {
    Investment* ptr;
    constexpr int size = sizeof...(params);
    if constexpr (size == 1) {
        ptr = new Stock{std::forward<T>(params)...};
    } else if constexpr (size == 2) {
        ptr = new Bond{std::forward<T>(params)...};
    } else if constexpr (size == 3) {
        ptr = new ReadEstate{std::forward<T>(params)...};
    }
    return ptr;
}

// 使用智能指针
// ptr 不会悬垂引用, 也不需要手动释放
template <typename... T>
std::unique_ptr<Investment> makeInvestment_test3(T&&... params) {
    constexpr int size = sizeof...(params);
    std::unique_ptr<Investment> ptr{nullptr};
    if constexpr (size == 1) {
        ptr.reset(new Stock{std::forward<T>(params)...});
    } else if constexpr (size == 2) {
        ptr.reset(new Bond{std::forward<T>(params)...});
    } else if constexpr (size == 3) {
        ptr.reset(new ReadEstate{std::forward<T>(params)...});
    }
    return ptr;
}

auto casa1() {
    util::note("演示在栈上分配内存, 会导致悬垂引用, 就是传出来的 ptr 会指向一个已经释放的内存");
    {
        auto ptr = makeInvestment_test1(1);
        std::cout << "ptr: " << ptr << std::endl;
        // delete ptr; // 这里会报错, 因为ptr是栈上的内存, 不能手动释放
    }

    util::note("演示在堆上分配内存, 使用原始指针, 需要手动释放");
    {
        auto ptr = makeInvestment_test2(1);
        std::cout << "ptr: " << ptr << std::endl;
        delete ptr;
    }

    util::note("演示在堆上分配内存, 但是使用智能指针, 不需要手动释放");
    {
        auto ptr = makeInvestment_test3(1);
        std::cout << "ptr: " << ptr.get() << std::endl;
    }
}

int main() {
    util::title("对于独占资源使用std::unique_ptr");

    util::subtitle("使用智能指针的原因");
    casa1();
}

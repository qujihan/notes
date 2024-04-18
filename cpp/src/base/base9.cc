#include "utils/util.h"

const size_t SIZT_T_MAX = 0xffffffffffffffff;

template <typename T>
class Array {
public:
    // 构造函数的异常捕获
    Array(size_t n) try : size_(n), data_(new T[n]) {
    } catch (const std::bad_alloc& ba) {
        std::cout << "no enouth memory." << std::endl;
        throw;
    }

private:
    size_t size_;
    T* data_;
};

void mystrcpy(char* dest, const char* src) {
    if (dest == nullptr || src == nullptr) {
        throw std::invalid_argument("dest or src is nullptr.");
    }
    while ((*dest++ = *src++) != '\0') {
    }
    *dest = '\0';
}

void exception_demo() {
    std::cout << "使用函数会异常的例子" << std::endl;
    char* dest = nullptr;
    const char* src = "hello";
    // 在被 catch 的时候, 才会发生栈展开
    try {
        mystrcpy(dest, src);
    } catch (const std::logic_error& e) {
        std::cout << "in catch" << std::endl;
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "in ... catch" << std::endl;
        throw;
    }
    std::cout << std::endl;

    std::cout << "使用 new 会异常的例子" << std::endl;
    std::cout << "即将分配 N: " << SIZT_T_MAX << std::endl;
    try {
        Array<double> array{SIZT_T_MAX};
    } catch (const std::bad_alloc& ba) {
        std::cout << "main catch" << std::endl;
    } catch (...) {
        std::cout << "in ... catch" << std::endl;
        throw;
    }
}

template <typename T>
class Buffer {
public:
    explicit Buffer(size_t n) : length_(0), capacity_(n) {
        std::cout << "Buffer::Buffer(size_t)" << std::endl;
        data_ = capacity_ == 0 ? nullptr : new T[capacity_]{};
    }

    ~Buffer() {
        std::cout << "Buffer::~Buffer()" << std::endl;
        delete[] data_;
    }

    Buffer(const Buffer& buffer) {
        std::cout << "Buffer::Buffer(const Buffer&)" << std::endl;
        this->length_ = buffer.length_;
        this->capacity_ = buffer.capacity_;
        this->data_ = capacity_ == 0 ? nullptr : new T[capacity_]{};
        std::copy(buffer.data_, buffer.data_ + buffer.capacity_, this->data_);
    }

    Buffer(Buffer&& buffer) noexcept {
        std::cout << "Buffer::Buffer(Buffer&&)" << std::endl;
        this->length_ = buffer.length_;
        this->capacity_ = buffer.capacity_;
        this->data_ = buffer.data_;
        buffer.length_ = 0;
        buffer.capacity_ = 0;
        buffer.data_ = nullptr;
    }

    Buffer& operator=(const Buffer& buffer) {
        std::cout << "Buffer::operator=(const Buffer&)" << std::endl;
        if (this != &buffer) {
            // 把自己之前的free掉
            delete[] data_;
            this->length_ = buffer.length_;
            this->capacity_ = buffer.capacity_;
            this->data_ = capacity_ == 0 ? nullptr : new T[capacity_]{};
            std::copy(buffer.data_, buffer.data_ + buffer.capacity_, this->data_);
        }
        return *this;
    }

    Buffer& operator=(Buffer&& buffer) noexcept {
        std::cout << "Buffer::operator=(Buffer&&)" << std::endl;
        if (this != &buffer) {
            // 把自己之前的free掉
            delete[] data_;
            this->length_ = buffer.length_;
            this->capacity_ = buffer.capacity_;
            this->data_ = buffer.data_;
            buffer.length_ = 0;
            buffer.capacity_ = 0;
            buffer.data_ = nullptr;
        }
        return *this;
    }

    void change_capacity(size_t n) {
        std::cout << "Buffer::change_capacity(sizt_t)" << std::endl;
        std::cout << "NOTE:这里做演示使用, 实际实现中往往伴随着内存分配, 这里没有内存分配" << std::endl;
        capacity_ = n;
    }

private:
    size_t length_;
    size_t capacity_;
    T* data_;
};

// SEABuffer = StrongExceptionSafeBuffer
// 强异常安全保证
//
// 思想是: 在异常发生之前, 不会对原有的数据做任何修改
// **修改的只是临时变量**, 这些变量在函数没有执行成功会自动析构
// 可以看到, 异常全在构造函数里面
template <typename T>
class SEABuffer {
public:
    template<typename U>
    friend void swap(SEABuffer<U>& lhs, SEABuffer<U>& rhs);

    explicit SEABuffer(size_t n) : length_(0), capacity_(n) {
        std::cout << "SEABuffer::SEABuffer(sizt_t)" << std::endl;
        data_ = capacity_ == 0 ? nullptr : new T[capacity_]{};
    }
    ~SEABuffer() {
        std::cout << "SEABuffer::~SEABuffer()" << std::endl;
        delete[] data_;
    }

    SEABuffer(const SEABuffer& buffer)
            : length_(buffer.length_),
              capacity_(buffer.capacity_),
              data_(capacity_ == 0 ? nullptr : new T[capacity_]{}) {
        std::cout << "SEABuffer::SEABuffer(const SEABuffer&)" << std::endl;
        std::copy(buffer.data_, buffer.data_ + buffer.capacity_, data_);
    }

    SEABuffer(SEABuffer&& buffer) noexcept : SEABuffer(0) {
        std::cout << "SEABuffer::SEABuffer(SEABuffer&&)" << std::endl;
        swap(*this, buffer);
    }

    // 这个函数特别巧妙
    // 无论是左值还是右值, 都会调用这个函数
    // 如果传左值, 会执行复制构造函数
    // 如果传右值, 会执行移动构造函数
    SEABuffer& operator=(SEABuffer buffer) {
        std::cout << "SEABuffer::operator=(SEABuffer)" << std::endl;
        swap(*this, buffer);
        return *this;
    }

    void change_capacity(size_t n) {
        std::cout << "SEABuffer::change_capacity(sizt_t)" << std::endl;
        std::cout << "NOTE:这里做演示使用, 实际实现中往往伴随着内存分配, 这里没有内存分配" << std::endl;
        capacity_ = n;
    }

private:
    size_t length_;
    size_t capacity_;
    T* data_;
};

template <typename T>
void swap(SEABuffer<T>& lhs, SEABuffer<T>& rhs) {
    // 这句的意思是, 如果有专门为这个类写的 swap 函数, 就调用那个
    // 如果没有, 就会调用 std::swap
    //
    // **优先匹配自定义的 swap 函数**
    //
    using std::swap;
    swap(lhs.data_, rhs.data_);
    swap(lhs.capacity_, rhs.capacity_);
    swap(lhs.length_, rhs.length_);
}

// 异常安全保证
void exception_safety_grarantee() {
    std::cout << "异常安全保证有这么几种" << std::endl;
    std::cout << "1. 不抛出 保证: 保证一定不会抛出异常(noexcept 关键字)" << std::endl;
    std::cout << "2. 强异常安全保证 保证: 抛出的异常不会改变程序的任何状态" << std::endl;
    std::cout << "3. 弱异常安全 保证: 抛出的异常会改变状态, 但是都是有效状态" << std::endl;
    std::cout << std::endl;

    std::cout << "Buffer 是一个弱异常安全保证的类, 我们尝试使用代码去看一下" << std::endl;
    Buffer<int> buffer{1};
    buffer.change_capacity(SIZT_T_MAX); // 此处无论是复制还是移动构造, 都会抛出异常
    std::cout << std::endl;

    std::cout << "拷贝构造类, 会抛出异常" << std::endl;
    try {
        Buffer<int> copyBuffer{buffer};
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "移动构造类, 会抛出异常" << std::endl;
    try {
        Buffer<int> copyBuffer{std::move(buffer)};
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

int main(){
    util::subtitle("异常使用的例子");
    exception_demo();

    util::subtitle("异常安全保证");
    exception_safety_grarantee();
}

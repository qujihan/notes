#include <utility>

#include "utils/util.h"

class Widget {
public:
    Widget() { std::cout << "Widget()" << std::endl; }
    Widget(Widget&& rhs) : name(std::move(rhs.name)) { std::cout << "Widget(Widget&& rhs)" << std::endl; }
    Widget(const Widget& rhs) : name(rhs.name) { std::cout << "Widget(const Widget& rhs)" << std::endl; }

    // 通用引用
    // template <typename T>
    // void setName(T&& newName) {
    //     name = std::forward<T>(newName);
    //     // name = std::move(newName);
    // }


    // 下面这两个setName函数, 一个是右值引用, 一个是左值引用
    // 这样写会有两个问题
    // 1. 现在是1个参加, 需要两个函数, 如果n个参数, 就需要写2^n个函数
    // 2. 不够高效
    //    1. 
    //    2. 

    void setName(std::string&& newName) {
        std::cout << "move setName" << std::endl;
        name = std::move(newName);
    }

    void setName(const std::string& newName) {
        std::cout << "copy setName" << std::endl;
        name = newName;
    }

private:
    std::string name;
};

auto case1() {
    std::string name = "name";
    Widget w;
    w.setName(name);
    w.setName(std::move(name));
    // "name" 其实是左值, 但是这里调用了 move setName
    // 因为 "name" 构建了一个右值std::string, 进而调用了 move setName
    w.setName("name");
}

int main() {
    util::title("对右值引用使用std::move, 对通用引用使用std::forward");
    case1();
}

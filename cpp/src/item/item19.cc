#include <memory>
#include <vector>

#include "utils/util.h"

class Widget;
std::vector<std::shared_ptr<Widget>> processedWidgets;

class Widget : public std::enable_shared_from_this<Widget> {
public:
    template <typename... T>
    static std::shared_ptr<Widget> create(T&&... args) {
        return std::shared_ptr<Widget>(new Widget(std::forward<T>(args)...));
    }
    void process() { processedWidgets.emplace_back(shared_from_this()); }

private:
    Widget(int a) : a(a) {}
    int a;
};

auto case1() {
    {
        auto pw = Widget::create(1);
        pw->process();
    }
}

int main() {
    util::title("对于共享资源使用std::shared_ptr");

    util::subtitle("double free");
    case1();
}

#include <memory>

#include "utils/util.h"

class Widget {};
void processWidget(std::shared_ptr<Widget> p, int priority);
int get_priority() {
    // do something
    // may throw exception
    return 1;
}

auto case1() {
    // get_priority()返回值可能抛出异常, 但是Widget对象已经创建, 会造成内存泄漏
    // 其实这里的本质问题是: std::shared_ptr<Widget> p(new Widget)是两个步骤, 可能会被异常打断
    processWidget(std::shared_ptr<Widget>(new Widget), get_priority());
    // 使用std::make_shared可以避免这个问题
    processWidget(std::make_shared<Widget>(), get_priority());
}

int main() {
    util::title("优先考虑使用std::make_unique和std::make_shared, 而非直接使用new");
}

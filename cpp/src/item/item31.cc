#include <functional>
#include <vector>

#include "utils/util.h"

auto addDivisorFilter() {
    using FilterContainer = std::vector<std::function<bool(int)>>;
    FilterContainer filters;
    int divisor = 3;
    filters.emplace_back([&](int val) { return val % divisor == 0; });
}

class Widget {
    using FilterContainer = std::vector<std::function<bool(int)>>;

public:
    void addFilter() {
        // 此时捕获了this指针, 如果Widget对象析构了, 会导致this的空指针
        filters.emplace_back([=](int val) { return val % divisor == 0; });

        // 这样会报错, 'divisor' in capture list does not name a variable
        // filters.emplace_back([divisor](int val) { return val % divisor == 0; });

        // after c++14 可以这么写, 这样不会捕获 this 指针
        filters.emplace_back([divisor = divisor](int val) { return val % divisor == 0; });
    }

private:
    FilterContainer filters;
    int divisor;
};

int main() {}

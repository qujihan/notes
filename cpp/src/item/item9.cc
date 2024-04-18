#include "utils/util.h"

struct test1 {
    static int suboptimal;
};

struct test2 {
    typedef int suboptimal;
};

template <typename T>
class TClass {
public:
    void foo() { T::suboptimal = 1; }
};

template <class T>
using myvector = std::vector<T>;

template <typename T>
struct myvector2 {
    typedef std::vector<T> type;
};

int main() {
    myvector<double> a;
    a.push_back(1.0);
    myvector2<double>::type b;
    b.push_back(1.0);
}

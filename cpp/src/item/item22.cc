#include "utils/util.h"
#include "item22_include/Widget.h"

int main(){
    util::title("当使用Pimpl惯用法, 请在实现文件中定义特殊成员函数");
    {
        Widget w;
        Widget w2(std::move(w));
    }
}

#include "Widget.h"

#include <vector>

#include "Gadget.h"

struct Widget::Impl {
    Gadget g1, g2, g3;
};
Widget::Widget() : pImpl(std::make_unique<Impl>()) {}
Widget::~Widget() = default;
Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(Widget&& rhs) = default;

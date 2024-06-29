# 类型推导
## 条款 1:  模板类型推导
在这个条款中主要是为了理解 模板类型 T 与 const/*/&/&& 结合在一起去推导类型的时候, T 本身会被推导为什么类型

这里需要注意的两点:
- const T* 与 T const* 是相同的类型, const 与 * 在一起作用的使用时, 看的是他们两个的相对位置, 与 T 的位置无关
- 在代码中使用的 typeid(T) 来判断 T 的类型, 但是使用 typeid(T) 会忽略 顶层const 以及 引用
    - 至于为什么, 我的理解是:

- T
- T*
- T&
- T&&
- const T*
- const T&
- const T&&

## 条款 2:  auto 类型推导
- 万能引用`auto&&`
- auto 的不同类型推导, 例子:case1.
- auto 的类型推导走的是模板类型推导的路子, 所以函数接受参数时不能推导出 initializer_list, 例子:case2.
- {} 的模板类型推导.
- auto可以作为返回值, 走的模板的方式
```cpp
// 明显是错的, auto 需要指明类型
auto func(){
    reutrn {1,2,3};
}
```
## 条款 3: 理解decltype
- decltype + 变量: 所有的信息都会保留, 数组也不会退化成指针
- decltype + 表达式: 根据表达式结果的类型返回
  - 返回值是 左值: 返回其引用
  - 返回值是 右值: 返回该类型

- 如果一个变量想获取表达式属性, 可以加()
  - decltype(a): 这个是 decltype + 变量
  - decltype((a)): 这个是 decltype + 表达式

有几个点需要注意一下:
1. 变量有底层 const, 表达式没有 const
```c++
int a = 10;
const int * const cacptr = &a;
// mytyep Type: const int&
decltype(*cacptr) mytype{};
```
2. decltype 是一个编译期的操作
```c++
// 不会执行 func(10) 这个函数
decltype(func(10));
```

decltype 的使用场景: 主要给模板使用
1. 有时返回类型并不是能提前知道
2. 使用 auto 会走模板推导的路线, 再使用 decltype 的时候就能保留所有的信息
```c++
// 例子可以参考 item23
decltype(auto) func(){
    // ...
}
```

## 条款 4: 学会查看类型推导的结果


# auto
## 条款 5: 优先考虑auto而非显式类型声明
- 为什么要使用 auto
  - 有的类型名字太长了
  - 在C++14以后, 在 lambda 的性参中可以可以使用 auto 的.
    - `auto func = [](const std::unique_ptr<A> &p){}`
    - `auto func = [](auto &p){}`
  - lambda 表达式的返回值一定要使用 auto
  - 类型接收不对可能会造成错误
    - `std::vector<int> v; auto sz = v.size();`
    - `std::vector<int> v; unsigned sz = v.size();`
    - 第一个的类型是 std::vector<int>::size_type, 第二个是 unsigned
  - 避免类型写错造成的无意义的拷贝
    - `int a = 10; const float &b = a;`
    - `int a = 10; float temp = a; const float&b = temp;`
    - 两个相等, 但是多了一次拷贝, 如果使用 auto 就可以避免



## 条款 6: auto推导若非己愿，使用显式类型初始化惯用法
- 不可见的代理类会使得 auto 出来的结果并非己愿
- 那么需要自己显式转换

在 item6.cc 中使用 std::vector<bool> 来举例说明这个问题

# 移步现代C++
## 条款 7:  区别使用()以及{}创建对象
这一部分主要是为了理解 {} 存在的意义以及好处.
这一部分可以参考 [这里](http://josuttis.com/cpp/c++initialization.pdf)

使用 {} 的好处:
- 减少拷贝/移动, 例子: case1,case2.
- 不允许缩窄转换, 例子: 不允许 double -> int 这样的转换.
- 减少了聚合类的初始化.
- 在使用std::array的时候需要多包一层{}, 例子, case3
- 免疫了C++的解析问题.
    - 因为C++中任意变量可以被任意多括号包裹, 所以不能分清函数调用和声明.
```c++
// 1
MyClass a;
int func_or_call(MyClass(a));
int func_or_call(MyClass a);

// 2
struct A{
public:
    // 这个不能通过编译
    int a(0);
    // 这个就可以
    int a{0}
}
```

Note:
- 在C++14以后, `A{a}` 与 `A = {a}` 不等价了
## 条款 8: 优先考虑 nullptr 而不是 NULL / 0
- nullptr / NULL / 0 的类型不同
  - `auto a = 0; // a 是 int`
  - `auto a = NULL; // a 是 long`
  - `auto a = nullptr; // a 是 std::nullptr_t`
- 使用 nullptr 可以正确的调用函数重载

## 条款 9: 优先考虑别名声明而不是typedef
- 对于作用域运算符`::`, 当用在泛型`T`的时候, 会被解析成static对象, 如果想要解析成类型, 需要加上 `typedef`
- 另外使用 using 比较简洁
```c++
template<class T>
using myvector = std::vector<T>;

template<typename T>
struct myvector{
    typedef std::vector<T> type;
};

int main(){
    myvector<double> a;
    a.push_back(1.0);

    // 这个样子多麻烦, 所以还是使用 using 而不是 typedef
    myvector2<float>::type b;
    b.push_back(1.0);
}
```

## 条款 10:
## 条款 11:
## 条款 12:
## 条款 13:
## 条款 14:
## 条款 15: 尽可能的使用constexpr
- constexpr 值都是编译器的常量
- constexpr 函数
  - C++11的限制
    - 普通函数
      - 1. 返回值不能是 void
      - 2. 函数体只能是 return expr; (只能一句话)
        - expr 必须是常量表达式
        - 如果函数存在行参, 那么替换到 expr 中也需要是常量表达式
          - `constexpr int func(int x){retrun x++;} // 不行`
        - (如果给constexpr传运行时的参数, 那么这个函数就退化成了普通函数)
          - 主要是为了简化代码
    - 构造函数
      - 3. 初始化列表中也必须是常量表达式
      - 4. 函数体必须是空的
      - 5. 所有与这个类相关的成员的, 析构函数必须是默认的
    - 类成员函数
      - 6. constexpr 声明的成员函数具有 const 属性
        - `constexpr int func(){}`
        - `constexpr int func() const{}`
        - 上面两个等价, 就是不能修改类中的值
  - C++14 的改进
    - 移除 1/2/4/6 这几条规则
    - 函数可以修改生命周期与常量表达式相同的对象
  - if constexprt(since C++17)

## 条款 16:
## 条款 17: 理解特殊成员函数的生成
- 特殊成员函数
  - 默认构造 / ①析构 / ②拷贝构造 / ③拷贝赋值 (C++98及之前)
  - ④移动构造 / ⑤移动赋值 (C++11及以后)

- 三五法则
  - 如果手动声明了 ①②③④⑤ 之间的一个, 那么大概率需要生成其他几个

- 历史遗留问题
  - 在C++98以前, 生成了①②③中的一个, 理论上其他几个需要手动生成, 不应该由编译器自动生成, 但是编译器还是会自动生成, 这个非常危险
  - 在C++11以后, 为了兼容, 也为了改进(及时止损), 添加了以下规则
    - 声明了 ①②③, 就不会声明 ④⑤ 中其他的
    - 声明了 ④⑤, 就不会声明 ②③④⑤ 中其他的(析构会生成)
# 智能指针
## 条款 18:
## 条款 19:
## 条款 20:
## 条款 21:
## 条款 22:
# 右值引用 / 移动语义 / 完美转发
## 条款 23: 理解 std::move 以及 std::forword
- std::move 执行到右值的无条件转换, 但是并没有移动任何东西
- std::forward 只有在参数绑定为右值的时候, 才会转换成右值
- std::move 和 std::forward 在运行期什么都没有做

## 条款 24:
## 条款 25:
## 条款 26:
## 条款 27:
## 条款 28:
## 条款 29:
## 条款 30:
## 条款 31:
# Lambda
## 条款 32:
## 条款 33:
## 条款 34:
# 并发
## 条款 35:
## 条款 36:
## 条款 37:
## 条款 38:
## 条款 39:
## 条款 40:
## 条款 41:
## 条款 42:
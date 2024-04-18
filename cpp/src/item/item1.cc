#include <functional>

#include "utils/util.h"
//
// 定义的全局变量
//
int a = 10;
int* int_ptr = &a;
int& int_ref = a;
int&& int_rref = std::move(a);

const int const_int = a;
const int* const_int_ptr = &const_int;
const int& const_int_ref = const_int;
const int&& const_int_rref = std::move(const_int);

int* const int_const_ptr = &a;
const int* const const_int_const_ptr = &a;

int array[2] = {0, 1};
const int const_array[2] = {0, 1};
int (*array_ptr)[2] = &array;
int (&array_ref)[2] = array;

void func(int a, int b) {
    std::cout << "func" << std::endl;
}
void (*func_ptr)(int, int) = func;
void (&func_ref)(int, int) = func;

//
// 10 种情况
//
template <typename T>
void f_T(T t) {
    std::cout << "T t, type(T)=" << util::get_type_name(typeid(T).name()) << std::endl;
}

template <typename T>
void f_T_ptr(T* t) {
    std::cout << "T* t, type(T)=" << util::get_type_name(typeid(T).name()) << std::endl;
}

template <typename T>
void f_T_ref(T& t) {
    std::cout << "T& t, type(T)=" << util::get_type_name(typeid(T).name()) << std::endl;
}

template <typename T>
void f_T_rref(T&& t) {
    std::cout << "T&& t, type(T)=" << util::get_type_name(typeid(T).name()) << std::endl;
}

// f(T t) 与 f(const T t) 不构成重载
// 意思就是底层 const 不构成重载
// 实际实验的时候忽略这种情况
template <typename T>
void f_const_T(const T t) {
    std::cout << "const T t, type(T)=" << util::get_type_name(typeid(T).name()) << std::endl;
}

template <typename T>
void f_const_T_ptr(const T* t) {
    std::cout << "const T* t, type(T)=" << util::get_type_name(typeid(T).name()) << std::endl;
}

template <typename T>
void f_const_T_ref(const T& t) {
    std::cout << "const T& t, type(T)=" << util::get_type_name(typeid(T).name()) << std::endl;
}

template <typename T>
void f_const_T_rref(const T&& t) {
    std::cout << "const T&& t, type(T)=" << util::get_type_name(typeid(T).name()) << std::endl;
}

// f(T* t) 与 f(T* const t) 不构成重载
// 意思就是底层 const 不构成重载
// 实际实验的时候忽略这种情况
template <typename T>
void f_T_ptr_const(T* const t) {
    std::cout << "T* const t, type(T)=" << util::get_type_name(typeid(T).name()) << std::endl;
}

// f(const T* t) 与 f(const T* const t) 不构成重载
// 意思就是底层 const 不构成重载
// 实际实验的时候忽略这种情况
template <typename T>
void f_const_T_ptr_const(const T* const t) {
    std::cout << "const T* const t, type(T)=" << util::get_type_name(typeid(T).name()) << std::endl;
}

void f_T_demo() {
    util::subtitle("T t");
    util::comment("当T没有任何修饰的时候, 使用的是传值(pass by value)");
    util::comment("这也就意味着自身的const是可以忽略的");
    util::comment("但是被指向的东西的const属性不能忽略");

    util::note("没有const的情况");
    util::comment("最常见的情况, 没什么好说的");
    f_T(a);
    util::comment("传入的是指针, 被理解为指针");
    f_T(int_ptr);
    util::comment("传入值而已, 无所谓是不是引用");
    f_T(int_ref);
    util::comment("传入值而已, 无所谓是不是引用");
    f_T(int_rref);

    util::note("存在修饰指向对象的const");
    util::comment("这意味传入的值不能修改, 但是我们这里值传递, 可以忽略这个const");
    f_T(const_int);
    util::comment("这意味这个指针指向的值不能修改, 我们把指针的值当作值传递过去了, 需要加一个const来保持这个属性");
    f_T(const_int_ptr);
    util::comment("同上面的引用传递");
    f_T(const_int_ref);
    util::comment("同上面的引用传递");
    f_T(const_int_rref);

    util::note("存在修饰自身的const");
    util::comment("这里是要求不能改指针所指向的值, 但是我们是值传递, 无所谓, 可以忽略这个const");
    f_T(int_const_ptr);
    util::comment("这是上面两种const的结合");
    f_T(const_int_const_ptr);

    util::note("数组和函数指针的情况");
    util::comment("传入数组会退化成传入T*");
    f_T(array);
    util::comment("如果传入const数组会退化成传入const T const*");
    util::comment("但是本身的const会被忽略, 所以成了const T*");
    f_T(const_array);
    f_T(array_ptr);
    f_T(array_ref);
    f_T(func);
    f_T(func_ptr);
    f_T(func_ref);
}

void f_T_ptr_demo() {
    util::subtitle("T* t");

    util::note("没有const的情况");
    // f_T_ptr(a);
    f_T_ptr(int_ptr);
    // f_T_ptr(int_ref);
    // f_T_ptr(int_rref);

    util::note("存在修饰指向对象的const");
    // f_T_ptr(const_int);
    util::comment("下面实际上是 const int");
    f_T_ptr(const_int_ptr);
    // f_T_ptr(const_int_ref);
    // f_T_ptr(const_int_rref);

    util::note("存在修饰自身的const");
    f_T_ptr(int_const_ptr);
    util::comment("下面T的实际推导类型是 const int");
    f_T_ptr(const_int_const_ptr);

    util::note("数组和函数指针的情况");
    f_T_ptr(array);
    util::comment("下面T的实际推导类型是 const int");
    f_T_ptr(const_array);
    f_T_ptr(array_ptr);
    f_T_ptr(array_ref);
    f_T_ptr(func);
    f_T_ptr(func_ptr);
    f_T_ptr(func_ref);
}

void f_T_ref_demo() {
    util::subtitle("T& t");
    util::comment("如果传入的是引用, 那么就忽略这个引用");
    util::comment("然后再去匹配");

    util::note("没有const的情况");
    f_T_ref(a);
    f_T_ref(int_ptr);
    f_T_ref(int_ref);
    f_T_ref(int_rref);

    util::note("存在修饰指向对象的const");
    util::comment("下面T的实际推导类型是 const int");
    util::comment("这么理解: 传入的是一个不希望修改的引用, 如果想要达成这个目的, 就只能使用const int");
    f_T_ref(const_int);
    f_T_ref(const_int_ptr);
    util::comment("下面T的实际推导类型是 const int");
    util::comment("这么理解: 传入的是一个不希望修改的引用, 如果想要达成这个目的, 就只能使用const int");
    f_T_ref(const_int_ref);
    util::comment("下面T的实际推导类型是 const int");
    util::comment("这么理解: 传入的是一个不希望修改的引用, 如果想要达成这个目的, 就只能使用const int");
    f_T_ref(const_int_rref);

    util::note("存在修饰自身的const");
    util::comment("下面T的实际推导类型是 int* const");
    util::comment("这里可以理解为, 传入的值在声明的时候不想被修改, 所以只能是int* const");
    f_T_ref(int_const_ptr);
    util::comment("下面T的实际推导类型是 const int* const/int const* const");
    f_T_ref(const_int_const_ptr);

    util::note("数组和函数指针的情况");
    f_T_ref(array);
    util::comment("下面T的实际推导类型是 const int[2]");
    f_T_ref(const_array);
    f_T_ref(array_ptr);
    f_T_ref(array_ref);
    f_T_ref(func);
    f_T_ref(func_ptr);
    f_T_ref(func_ref);
}

void f_T_rref_demo() {
    util::subtitle("T&& t");
    util::note("没有const的情况");
    util::comment("下面T的实际推导类型是 int&");
    f_T_rref(a);
    util::comment("下面T的实际推导类型是 int*&");
    f_T_rref(int_ptr);
    util::comment("下面T的实际推导类型是 int&");
    f_T_rref(int_ref);
    util::comment("下面T的实际推导类型是 int&");
    f_T_rref(int_rref);

    util::note("存在修饰指向对象的const");
    util::comment("下面T的实际推导类型是 const int&");
    f_T_rref(const_int);
    util::comment("下面T的实际推导类型是 const int*&");
    f_T_rref(const_int_ptr);
    util::comment("下面T的实际推导类型是 const int&");
    f_T_rref(const_int_ref);
    util::comment("下面T的实际推导类型是 const int&");
    f_T_rref(const_int_rref);

    util::note("存在修饰自身的const");
    util::comment("下面T的实际推导类型是 int* const&");
    f_T_rref(int_const_ptr);
    util::comment("下面T的实际推导类型是 const int* const&");
    f_T_rref(const_int_const_ptr);

    util::note("数组和函数指针的情况");
    util::comment("下面T的实际推导类型是 int(&)[2]");
    f_T_rref(array);
    util::comment("下面T的实际推导类型是 const int(&)[2]");
    f_T_rref(const_array);
    util::comment("下面T的实际推导类型是 int(*&)[2]");
    f_T_rref(array_ptr);
    util::comment("下面T的实际推导类型是 int(&)[2]");
    f_T_rref(array_ref);
    util::comment("下面T的实际推导类型是 void(&)(int, int)");
    f_T_rref(func);
    util::comment("下面T的实际推导类型是 void(*&)(int, int)");
    f_T_rref(func_ptr);
    util::comment("下面T的实际推导类型是 void(&)(int, int)");
    f_T_rref(func_ref);
}

void f_const_T_ptr_demo() {
    util::subtitle("const T* t");
    util::note("没有const的情况");
    // f_const_T_ptr(a);
    f_const_T_ptr(int_ptr);
    // f_const_T_ptr(int_ref);
    // f_const_T_ptr(int_rref);

    util::note("存在修饰指向对象的const");
    // f_const_T_ptr(const_int);
    f_const_T_ptr(const_int_ptr);
    // f_const_T_ptr(const_int_ref);
    // f_const_T_ptr(const_int_rref);

    util::note("存在修饰自身的const");
    f_const_T_ptr(int_const_ptr);
    f_const_T_ptr(const_int_const_ptr);

    util::note("数组和函数指针的情况");
    f_const_T_ptr(array);
    f_const_T_ptr(const_array);
    f_const_T_ptr(array_ptr);
    f_const_T_ptr(array_ref);
    // f_const_T_ptr(func);
    // f_const_T_ptr(func_ptr);
    // f_const_T_ptr(func_ref);
}

void f_const_T_ref_demo() {
    util::subtitle("const T& t");
    util::note("没有const的情况");
    f_const_T_ref(a);
    f_const_T_ref(int_ptr);
    f_const_T_ref(int_ref);
    f_const_T_ref(int_rref);

    util::note("存在修饰指向对象的const");
    f_const_T_ref(const_int);
    f_const_T_ref(const_int_ptr);
    f_const_T_ref(const_int_ref);
    f_const_T_ref(const_int_rref);

    util::note("存在修饰自身的const");
    f_const_T_ref(int_const_ptr);
    f_const_T_ref(const_int_const_ptr);

    util::note("数组和函数指针的情况");
    f_const_T_ref(array);
    f_const_T_ref(array_ptr);
    f_const_T_ref(array_ref);
    f_const_T_ref(func);
    f_const_T_ref(func_ptr);
    f_const_T_ref(func_ref);
}

void f_const_T_rref_demo() {
    util::subtitle("const T&& t");
    // f_const_T_rref(a);
    // f_const_T_rref(int_ptr);
    // f_const_T_rref(int_ref);
    // f_const_T_rref(int_rref);

    // f_const_T_rref(const_int);
    // f_const_T_rref(const_int_ptr);
    // f_const_T_rref(const_int_ref);
    // f_const_T_rref(const_int_rref);

    // f_const_T_rref(int_const_ptr);
    // f_const_T_rref(const_int_const_ptr);

    // f_const_T_rref(array);
    // f_const_T_rref(array_ptr);
    // f_const_T_rref(array_ref);

    util::note("数组和函数指针的情况");
    f_const_T_rref(func);
    // f_const_T_rref(func_ptr);
    f_const_T_rref(func_ref);
}

void sort_by_template() {
    f_T_demo();
    f_T_ptr_demo();
    f_T_ref_demo();
    f_T_rref_demo();
    f_const_T_ptr_demo();
    f_const_T_ref_demo();
    f_const_T_rref_demo();
}

int main() {
    util::title("理解模板类型推导");
    util::suggession("共T, T*, T&, T&&, const T*, const T&, const T&&需要考虑");
    util::suggession("const T* 与 T const* 是相同的!");
    util::warning("typeid(T)会忽略顶层const以及引用, 我会在他省略的时候备注上");
    sort_by_template();
}

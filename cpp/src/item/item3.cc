#include "utils/util.h"

void case1(){
    int a = 10;
    const int * const cacptr = &a;
    // mytyep Type: const int&
    decltype(*cacptr) mytype{};
}


int main(){}

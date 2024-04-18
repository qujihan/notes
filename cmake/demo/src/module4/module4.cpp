#include "module4.h"

#include "module2.h"
#include "module3.h"

void module_four() {
  std::cout << "\n this is module 4 \n" << std::endl;

  std::cout << "module4 use module2: " << std::endl;
  module_two();
  std::cout << "module4 use module3: " << std::endl;
  module_three();
}
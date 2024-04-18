#include "module3.h"

#include "module1.h"
#include "module2.h"

void module_three() {
  std::cout << "\n this is module 3 \n" << std::endl;

  std::cout << "module3 use module1: " << std::endl;
  module_one();
  std::cout << "module3 use module2: " << std::endl;
  module_two();
}
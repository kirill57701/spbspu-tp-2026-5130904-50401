#include <iostream>
#include "comms.hpp"

int main()
{
  petrov::knowbase A;
  A.reg_comm(std::cin, std::cout);
  return 0;
}

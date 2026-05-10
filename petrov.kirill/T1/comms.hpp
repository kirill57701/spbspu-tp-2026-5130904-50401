#ifndef COMMS_HPP
#define COMMS_HPP
#include <string>
#include <vector>

namespace petrov
{
  struct mem
  {
    mem(str::string s);
    std::string name;
    std::string lines;
    std::vector<std::weak_ptr<mem>> txt;
  };
  
}

#endif

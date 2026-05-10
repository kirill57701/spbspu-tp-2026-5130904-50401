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
    std::vector<str::string> lines;
    std::vector<std::weak_ptr<mem>> txt;
  };
  class knowbase
  {
    public:
      void reg_comm(std::iostream& in, std::ostream& ou);
    private:
      std::unordered_map
  };
}

#endif

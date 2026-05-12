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
      std::unordered_map <std::string, std::shared_ptr<mem>> storage;
      bool note(std::istream& in);
      bool line(std::istream& in);
      bool show(std::istream& in, std::ostream& ou);
      bool drop(std::istream& in);
      bool link(std::istream& in);
      bool mind(std::istream& in, std::ostream& ou);
      bool expiried(std::istream& in, std::ostream& ou);
      bool refresh(std::istream& in);
  };
}

#endif

#include <iostream>
#include <string>

namespace petrov
{
  struct DataStruct
  {
    size_t k1, k2;
    std::string k3;
  };
  struct ExpChar
  {
    char exp;
  };
  struct ReadOct
  {
    size_t& oct;
  };
  struct ReadHex
  {
    size_t& oct;
  };
  struct ReadStr
  {
    std::string& str;
  };

  class Format
  {
  public:
    explicit Format(std::basic_ios<char>& stream);
    ~Format();
  };
}

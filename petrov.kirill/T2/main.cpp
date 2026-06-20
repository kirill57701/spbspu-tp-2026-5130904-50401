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

  private:
    std::basic_ios<char>& stream_;
    char fill_;
    std::streamsize prec_;
    std::basic_ios<char>::fmtflags fl_;
  };

  std::istream& operator>>(std::istream& in, ExpChar&& d);
  std::istream& operator>>(std::istream& in, ReadOct&& d);
  std::istream& operator>>(std::istream& in, ReadHex&& d);
  std::istream& operator>>(std::istream& in, ReadStr&& d);
  std::istream& operator>>(std::istream& in, DataStruct& d);
  std::ostream& operator<<(std::ostream& out, const DataStruct& src);

  bool compareData(const DataStruct& lhs, const DataStruct& rhs);

  FormatGuard::FormatGuard(std::basic_ios<char>& stream):
      stream_(stream),
      fill_(stream.fill()),
      precision_(stream.prec()),
      fl_(stream.flags()) {
  }

  FormatGuard::~FormatGuard() {
    stream_.fill(fill_);
    stream_.precision(prec_);
    stream_.flags(fl_);
  }
}

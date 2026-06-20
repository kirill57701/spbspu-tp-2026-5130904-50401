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

  std::istream& operator>>(std::istream& in, ExpectChar&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c;
    if (in >> c && c != dest.expected) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, ReadOct&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char prefix_zero;
    in >> prefix_zero;
    if (prefix_zero != '0') {
      in.setstate(std::ios::failbit);
      return in;
    }
    const int next_char = in.peek();
    if (next_char >= '0' && next_char <= '7') {
      in >> std::oct >> dest.ref >> std::dec;
    } else {
      dest.ref = 0;
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, ReadHex&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char prefix_zero;
    char prefix_x;
    in >> prefix_zero >> prefix_x;
    if (prefix_zero != '0' || (prefix_x != 'x' && prefix_x != 'X')) {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> std::hex >> dest.ref >> std::dec;
    return in;
  }
}

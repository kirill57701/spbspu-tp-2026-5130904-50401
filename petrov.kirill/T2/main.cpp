#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>

namespace petrov
{
  struct DataStruct
  {
    unsigned long long key1;
    unsigned long long key2;
    std::string key3;
  };

  struct ExpectChar
  {
    char expected;
  };

  struct ReadOct
  {
    unsigned long long& ref;
  };

  struct ReadHex
  {
    unsigned long long& ref;
  };

  struct ReadString
  {
    std::string& ref;
  };

  class FormatGuard
  {
  public:
    explicit FormatGuard(std::basic_ios<char>& stream);
    ~FormatGuard();

  private:
    std::basic_ios<char>& stream_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags flags_;
  };

  std::istream& operator>>(std::istream& in, ExpectChar&& dest);
  std::istream& operator>>(std::istream& in, ReadOct&& dest);
  std::istream& operator>>(std::istream& in, ReadHex&& dest);
  std::istream& operator>>(std::istream& in, ReadString&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& src);
  bool compareData(const DataStruct& lhs, const DataStruct& rhs);

  FormatGuard::FormatGuard(std::basic_ios<char>& stream):
    stream_(stream),
    fill_(stream.fill()),
    precision_(stream.precision()),
    flags_(stream.flags())
  {}

  FormatGuard::~FormatGuard()
  {
    stream_.fill(fill_);
    stream_.precision(precision_);
    stream_.flags(flags_);
  }

  std::istream& operator>>(std::istream& in, ExpectChar&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c;
    if (in >> c && c != dest.expected)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, ReadOct&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char prefix_zero;
    in >> prefix_zero;
    if (prefix_zero != '0')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    const int next_char = in.peek();
    if (next_char >= '0' && next_char <= '7')
    {
      in >> std::oct >> dest.ref >> std::dec;
    }
    else
    {
      dest.ref = 0;
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, ReadHex&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char prefix_zero;
    char prefix_x;
    in >> prefix_zero >> prefix_x;
    if (prefix_zero != '0' || (prefix_x != 'x' && prefix_x != 'X'))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> std::hex >> dest.ref >> std::dec;
    return in;
  }

  std::istream& operator>>(std::istream& in, ReadString&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> ExpectChar{'"'}, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    DataStruct temp{0, 0, ""};
    in >> ExpectChar{'('} >> ExpectChar{':'};

    for (int i = 0; i < 3; ++i)
    {
      std::string key;
      std::getline(in, key, ' ');

      if (key == "key1")
      {
        in >> ReadOct{temp.key1};
      }
      else if (key == "key2")
      {
        in >> ReadHex{temp.key2};
      }
      else if (key == "key3")
      {
        in >> ReadString{temp.key3};
      }
      else
      {
        in.setstate(std::ios::failbit);
      }

      if (i < 2)
      {
        in >> ExpectChar{':'};
      }
    }

    in >> ExpectChar{':'} >> ExpectChar{')'};

    if (in)
    {
      dest = temp;
    }

    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& src)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }

    FormatGuard guard(out);

    out << "(:key1 ";
    if (src.key1 == 0)
    {
      out << "0";
    }
    else
    {
      out << "0" << std::oct << src.key1;
    }

    out << ":key2 0x" << std::hex << std::uppercase << src.key2
        << ":key3 \"" << src.key3 << "\":)";

    return out;
  }

  bool compareData(const DataStruct& lhs, const DataStruct& rhs)
  {
    if (lhs.key1 != rhs.key1)
    {
      return lhs.key1 < rhs.key1;
    }
    if (lhs.key2 != rhs.key2)
    {
      return lhs.key2 < rhs.key2;
    }
    return lhs.key3.length() < rhs.key3.length();
  }

}

int main() {
  using namespace petrov;
  std::vector<DataStruct> data;

  while (std::cin)
  {
    std::cin >> std::ws;
    if (std::cin.eof())
    {
      break;
    }

    DataStruct temp;
    if (std::cin >> temp)
    {
      data.push_back(temp);
    }
    else
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end(), compareData);
  std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));

  return 0;
}

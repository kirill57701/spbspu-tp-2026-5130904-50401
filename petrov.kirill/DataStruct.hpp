#ifndef PETROV_DATA_STRUCT_HPP
#define PETROV_DATA_STRUCT_HPP
#include <string>
#include <iostream>

namespace petrov
{
  struct DataStruct
  {
    size_t k1, k2;
    std::string k3;
  }
}

std::istream& in operator>>(std::istream& inp, DataStuct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);

bool compDatStr(const DataStruct& l, const DatStruct& r);
#endif

#ifndef FUNCS_HPP
#define FUNCS_HPP

#include "polygon.hpp"
#include <vector>
#include <map>
#include <string>
#include <iosfwd>

namespace petrov {
  void processArea(std::istream& in, std::ostream& out, const std::vector< Polygon >& data);
  void processMax(std::istream& in, std::ostream& out, const std::vector< Polygon >& data);
  void processMin(std::istream& in, std::ostream& out, const std::vector< Polygon >& data);
  void processCount(std::istream& in, std::ostream& out, const std::vector< Polygon >& data);
  void processMaxSeq(std::istream& in, std::ostream& out, const std::vector< Polygon >& data);
  void processRightShapes(std::istream& in, std::ostream& out, const std::vector< Polygon >& data);

  void skipLine(std::istream& in);

  using CommandAction = void (*)(std::istream&, std::ostream&, const std::vector< Polygon >&);
  std::map< std::string, CommandAction > createCommands();
}

#endif

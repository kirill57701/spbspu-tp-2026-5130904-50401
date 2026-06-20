#include "functions.hpp"
#include <iostream>
#include <iomanip>
#include <numeric>
#include <functional>
#include <algorithm>

namespace petrov {
  bool isEvenPoints(const Polygon& poly)
  {
    return getVertexCount(poly) % 2 == 0;
  }

  bool isOddPoints(const Polygon& poly)
  {
    return getVertexCount(poly) % 2 != 0;
  }

  bool isPointsEqual(const Polygon& poly, size_t target)
  {
    return getVertexCount(poly) == target;
  }

  void skipLine(std::istream& in)
  {
    in.clear();
    std::string discard;
    std::getline(in, discard);
  }

  void processArea(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
  {
    std::string arg;
    if (!(in >> arg)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    std::vector< Polygon > matched;
    using namespace std::placeholders;

    if (arg == "EVEN") {
      std::copy_if(data.begin(), data.end(), std::back_inserter(matched), isEvenPoints);
    } else if (arg == "ODD") {
      std::copy_if(data.begin(), data.end(), std::back_inserter(matched), isOddPoints);
    } else if (arg == "MEAN") {
      if (data.empty()) {
        out << "<INVALID COMMAND>\n";
        return;
      }
      matched = data;
    } else {
      try {
        size_t num = std::stoull(arg);
        if (num < 3) {
          out << "<INVALID COMMAND>\n";
          return;
        }
        std::copy_if(data.begin(), data.end(), std::back_inserter(matched), std::bind(isPointsEqual, _1, num));
      } catch (...) {
        out << "<INVALID COMMAND>\n";
        return;
      }
    }

    std::vector< double > areas;
    areas.reserve(matched.size());
    std::transform(matched.begin(), matched.end(), std::back_inserter(areas), getPolygonArea);
    double total = std::accumulate(areas.begin(), areas.end(), 0.0);

    std::ostream::fmtflags flags = out.flags();
    out << std::fixed << std::setprecision(1);
    if (arg == "MEAN") {
      out << (total / static_cast< double >(data.size())) << "\n";
    } else {
      out << total << "\n";
    }
    out.flags(flags);
  }

  bool compareArea(const Polygon& left, const Polygon& right)
  {
    return getPolygonArea(left) < getPolygonArea(right);
  }

  bool compareVertexCount(const Polygon& left, const Polygon& right)
  {
    return getVertexCount(left) < getVertexCount(right);
  }

  void processMax(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
  {
    std::string arg;
    if (!(in >> arg) || data.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::ostream::fmtflags flags = out.flags();
    if (arg == "AREA") {
      auto maxIt = std::max_element(data.begin(), data.end(), compareArea);
      out << std::fixed << std::setprecision(1) << getPolygonArea(*maxIt) << "\n";
    } else if (arg == "VERTEXES") {
      auto maxIt = std::max_element(data.begin(), data.end(), compareVertexCount);
      out << getVertexCount(*maxIt) << "\n";
    } else {
      out << "<INVALID COMMAND>\n";
    }
    out.flags(flags);
  }
}

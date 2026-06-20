#include "polygon.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>
#include <iterator>

namespace petrov {
  std::istream& operator>>(std::istream& in, CharSeparator&& sep)
  {
    std::istream::sentry guard(in);
    if (!guard) {
      return in;
    }
    char c = 0;
    if (in >> c && c != sep.expected) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Point& point)
  {
    std::istream::sentry guard(in);
    if (!guard) {
      return in;
    }
    in >> CharSeparator{'('} >> point.x >> CharSeparator{';'} >> point.y >> CharSeparator{')'};
    return in;
  }

  std::istream& operator>>(std::istream& in, Polygon& polygon)
  {
    std::istream::sentry guard(in);
    if (!guard) {
      return in;
    }
    size_t vertexCount = 0;
    if (!(in >> vertexCount) || vertexCount < 3) {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::vector< Point > tempVertices;
    tempVertices.reserve(vertexCount);
    for (size_t i = 0; i < vertexCount; ++i) {
      Point tempPoint;
      if (in >> tempPoint) {
        tempVertices.push_back(tempPoint);
      } else {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    polygon.points = std::move(tempVertices);
    return in;
  }
}

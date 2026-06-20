#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iostream>

namespace petrov {
  struct Point {
    int x;
    int y;
  };

  struct Polygon {
    std::vector< Point > points;
  };

  struct CharSeparator {
    char expected;
  };

  std::istream& operator>>(std::istream& in, CharSeparator&& sep);
  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);

  bool operator==(const Point& left, const Point& right);
  bool operator==(const Polygon& left, const Polygon& right);

  double getPolygonArea(const Polygon& poly);
  size_t getVertexCount(const Polygon& poly);
  bool hasRightAngle(const Polygon& poly);
}

#endif

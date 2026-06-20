#include "polygon.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>
#include <iterator>

namespace petrov {
  std::istream& operator>>(std::istream& in, CharSeparator&& sep)
  {
    std::istream::sentry guard(in);
    if (!guard)
    {
      return in;
    }
    char c = 0;
    if (in >> c && c != sep.expected)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Point& point)
  {
    std::istream::sentry guard(in);
    if (!guard)
    {
      return in;
    }
    in >> CharSeparator{'('} >> point.x >> CharSeparator{';'} >> point.y >> CharSeparator{')'};
    return in;
  }

  std::istream& operator>>(std::istream& in, Polygon& polygon)
  {
    std::istream::sentry guard(in);
    if (!guard)
    {
      return in;
    }
    size_t vertexCount = 0;
    if (!(in >> vertexCount) || vertexCount < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::vector< Point > tempVertices;
    tempVertices.reserve(vertexCount);
    for (size_t i = 0; i < vertexCount; ++i)
    {
      Point tempPoint;
      if (in >> tempPoint)
      {
        tempVertices.push_back(tempPoint);
      }
      else
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    polygon.points = std::move(tempVertices);
    return in;
  }

  bool operator==(const Point& left, const Point& right)
  {
    return left.x == right.x && left.y == right.y;
  }

  bool operator==(const Polygon& left, const Polygon& right)
  {
    if (left.points.size() != right.points.size())
    {
      return 0;
    }
    return std::equal(left.points.begin(), left.points.end(), right.points.begin());
  }

  double getPolygonArea(const Polygon& poly)
  {
    size_t size = poly.points.size();
    if (size < 3)
    {
      return 0.0;
    }
    std::vector< size_t > indices(size);
    std::iota(indices.begin(), indices.end(), 0);
    std::vector< double > partialAreas;
    partialAreas.reserve(size);

    std::transform(indices.begin(), indices.end(), std::back_inserter(partialAreas),
      [&poly, size](size_t i)
      {
        const Point& p1 = poly.points[i];
        const Point& p2 = poly.points[(i + 1) % size];
        return static_cast< double >(p1.x * p2.y) - static_cast< double >(p2.x * p1.y);
      });

    double total = std::accumulate(partialAreas.begin(), partialAreas.end(), 0.0);
    return std::abs(total) / 2.0;
  }

  size_t getVertexCount(const Polygon& poly)
  {
    return poly.points.size();
  }

  bool checkAngle(const Point& p1, const Point& p2, const Point& p3)
  {
    long long dx1 = static_cast< long long >(p2.x) - p1.x;
    long long dy1 = static_cast< long long >(p2.y) - p1.y;
    long long dx2 = static_cast< long long >(p3.x) - p2.x;
    long long dy2 = static_cast< long long >(p3.y) - p2.y;
    return (dx1 * dx2 + dy1 * dy2) == 0;
  }

  bool hasRightAngle(const Polygon& poly)
  {
    size_t size = poly.points.size();
    if (size < 3)
    {
      return 0;
    }
    std::vector< size_t > indices(size);
    std::iota(indices.begin(), indices.end(), 0);
    return std::any_of(indices.begin(), indices.end(), [&](size_t i)
    {
      return checkAngle(poly.points[i], poly.points[(i + 1) % size], poly.points[(i + 2) % size]);
    });
  }
}

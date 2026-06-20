#include "funcs.hpp"
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

  void processMin(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
  {
    std::string arg;
    if (!(in >> arg) || data.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::ostream::fmtflags flags = out.flags();
    if (arg == "AREA") {
      auto minIt = std::min_element(data.begin(), data.end(), compareArea);
      out << std::fixed << std::setprecision(1) << getPolygonArea(*minIt) << "\n";
    } else if (arg == "VERTEXES") {
      auto minIt = std::min_element(data.begin(), data.end(), compareVertexCount);
      out << getVertexCount(*minIt) << "\n";
    } else {
      out << "<INVALID COMMAND>\n";
    }
    out.flags(flags);
  }

  void processCount(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
  {
    std::string arg;
    if (!(in >> arg)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    using namespace std::placeholders;
    size_t count = 0;
    if (arg == "EVEN") {
      count = std::count_if(data.begin(), data.end(), isEvenPoints);
    } else if (arg == "ODD") {
      count = std::count_if(data.begin(), data.end(), isOddPoints);
    } else {
      try {
        size_t num = std::stoull(arg);
        if (num < 3) {
          out << "<INVALID COMMAND>\n";
          return;
        }
        count = std::count_if(data.begin(), data.end(), std::bind(isPointsEqual, _1, num));
      } catch (...) {
        out << "<INVALID COMMAND>\n";
        return;
      }
    }
    out << count << "\n";
  }

  void processMaxSeq(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
  {
    Polygon target;
    if (!(in >> target)) {
      skipLine(in);
      out << "<INVALID COMMAND>\n";
      return;
    }

    int nextChar = in.get();
    while (nextChar != '\n' && nextChar != EOF) {
      if (!std::isspace(nextChar)) {
        skipLine(in);
        out << "<INVALID COMMAND>\n";
        return;
      }
      nextChar = in.get();
    }

    if (data.empty()) {
      out << 0 << "\n";
      return;
    }

    std::vector< bool > flagsVector;
    flagsVector.reserve(data.size());
    using namespace std::placeholders;
    std::transform(data.begin(), data.end(), std::back_inserter(flagsVector),
      std::bind(std::equal_to< Polygon >(), _1, target));

    size_t currentMax = 0;
    auto startIter = flagsVector.begin();
    while (startIter != flagsVector.end()) {
      startIter = std::find(startIter, flagsVector.end(), true);
      if (startIter == flagsVector.end()) {
        break;
      }
      auto endIter = std::find(startIter, flagsVector.end(), false);
      size_t sequenceLength = std::distance(startIter, endIter);
      currentMax = std::max(currentMax, sequenceLength);
      startIter = endIter;
    }

    out << currentMax << "\n";
  }

  void processRightShapes(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
  {
    (void)in;
    size_t count = std::count_if(data.begin(), data.end(), hasRightAngle);
    out << count << "\n";
  }

  std::map< std::string, CommandAction > createCommands()
  {
    std::map< std::string, CommandAction > cmdMap;
    cmdMap["AREA"] = processArea;
    cmdMap["MAX"] = processMax;
    cmdMap["MIN"] = processMin;
    cmdMap["COUNT"] = processCount;
    cmdMap["MAXSEQ"] = processMaxSeq;
    cmdMap["RIGHTSHAPES"] = processRightShapes;
    return cmdMap;
  }
}
